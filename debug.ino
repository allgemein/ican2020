void printVec3(vec3 data){
	Serial.print(data.x,4);
	Serial.print(",");
	Serial.print(data.y,4);
	Serial.print(",");
	Serial.print(data.z,4);
	Serial.print(",");
}
void printVec3ln(vec3 data){
	Serial.print(data.x,4);
	Serial.print(",");
	Serial.print(data.y,4);
	Serial.print(",");
	Serial.println(data.z,4);
}
