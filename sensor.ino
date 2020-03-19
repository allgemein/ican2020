/*
   センサ読み取り用関数
*/

float getTemp() {
  /*温度測定*/
  int val = 0;
  float temp = 0;
  Wire.beginTransmission(0x18);
  Wire.write(0x13);
  Wire.endTransmission(false);
  Wire.requestFrom(0x18, 1, true);
  val += Wire.read();
  Wire.beginTransmission(0x18);
  Wire.write(0x12);
  Wire.endTransmission(false);
  Wire.requestFrom(0x18, 1, true);
  val = val << 8;
  val += Wire.read();
  temp = (val - 2096) / 50;
  return temp;
}

void accelInit() {
  byte tmp;
  // MMA8452の内部レジスタを設定します。
  tmp = MMA8452_ReadByte(MMA8452_CTRL_REG1);
  MMA8452_WriteByte(MMA8452_CTRL_REG1, tmp &
                    ~(MMA8452_CTRL_REG1_ACTV_BIT));
  MMA8452_WriteByte(MMA8452_XYZ_DATA_CFG,
                    (MMA8452_G_SCALE >> 2));
  tmp = MMA8452_ReadByte(MMA8452_CTRL_REG1);
  MMA8452_WriteByte(MMA8452_CTRL_REG1, tmp |
                    MMA8452_CTRL_REG1_ACTV_BIT);
}

void MMA8452_ReadByteArray(byte adrs, int datlen, byte *
                           dest)
{
  Wire.beginTransmission(MMA8452_ADRS);
  Wire.write(adrs);
  Wire.endTransmission(false);
  Wire.requestFrom(MMA8452_ADRS, datlen);
  while (Wire.available() < datlen);
  for (int x = 0 ; x < datlen ; x++) {
    dest[x] = Wire.read();
  }
}

byte MMA8452_ReadByte(byte adrs)
{
  Wire.beginTransmission(MMA8452_ADRS);
  Wire.write(adrs);
  Wire.endTransmission(false);
  Wire.requestFrom(MMA8452_ADRS, 1);
  while (!Wire.available());
  return (Wire.read());
}
void MMA8452_WriteByte(byte adrs, byte dat)
{
  Wire.beginTransmission(MMA8452_ADRS);
  Wire.write(adrs);
  Wire.write(dat);
  Wire.endTransmission();
}

vec3 getHeadAcclerelation() {
  /*頭の加速度*/
  MatchState ms;
  vec3 output;
  String inData = Serial1.readStringUntil('\r\n');
  ms.Target(const_cast<char*>(inData.c_str()));
  if (ms.Match("::rc=")) {
    bool flag = true;
    ms.Target(const_cast<char*>(inData.c_str()));
    if (!ms.Match("x=*%d+") > 0)ms.Match("x=*-%d+");
    if (!ms.Match("x=*%d+") && !ms.Match("x=*-%d+"))flag = false;
    int x = (inData.substring(ms.MatchStart + 2, ms.MatchStart + ms.MatchLength)).toInt();
    ms.Target(const_cast<char*>(inData.c_str()));
    if (!ms.Match("y=*%d+") > 0)ms.Match("y=*-%d+");
    if (!ms.Match("y=*%d+") && !ms.Match("y=*-%d+"))flag = false;
    int y = (inData.substring(ms.MatchStart + 2, ms.MatchStart + ms.MatchLength)).toInt();
    ms.Target(const_cast<char*>(inData.c_str()));
    if (!ms.Match("z=*%d+") > 0)ms.Match("z=*-%d+");
    if (!ms.Match("z=*%d+") && !ms.Match("z=*-%d+"))flag = false;
    int z = (inData.substring(ms.MatchStart + 2, ms.MatchStart + ms.MatchLength)).toInt();
    output = {x, y, z};
    if (flag)return output;
  }
  vec3 None = { -1, -1, -1};
  return None;
}

vec3 getWristAcclerelation() {
  /*手首の加速度センサ*/
  byte buf[6];
  vec3 output;
  // MMA8452の内部レジスタにある測定値を読み込みます。
  // X: g[0], Y: g[1], Z: g[2] に対応します。
  MMA8452_ReadByteArray(MMA8452_OUT_X_MSB, 6, buf);
  output.x = -(float((int((buf[0] << 8) | buf[1]) >> 4)) / ((1 << 11) /
               MMA8452_G_SCALE));
  output.y = -(float((int((buf[2] << 8) | buf[3]) >> 4)) / ((1 << 11) /
               MMA8452_G_SCALE));
  output.z = -(float((int((buf[4] << 8) | buf[5]) >> 4)) / ((1 << 11) /
               MMA8452_G_SCALE));
  Serial.print(output.x);
  Serial.print(",");
  Serial.print(output.y);
  Serial.print(",");
  Serial.println(output.z);
  return output;
}
