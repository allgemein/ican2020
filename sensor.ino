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
  return output;
}
