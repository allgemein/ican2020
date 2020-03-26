/*
   寝落ち検知アルゴリズム
 */

bool isNeochi(vec3 head, vec3 wrist, float temp, float pulse) {	
	//bool neochi = (millis()<5000?false:isChanging(head));
	bool neochi = (-0.6<wrist.z);
	if(neochi)digitalWrite(13,HIGH);
	else digitalWrite(13,LOW);
	return neochi;
}

bool isChanging(vec3 data){
	vec3 ave = headAve(data);
	//Serial.print(ave.x);
	//Serial.print(",");
	//Serial.print(ave.y);
	//Serial.print(",");
	//Serial.println(ave.z);
	if( threshold < abs(data.x - ave.x)
			|| threshold < abs(data.y - ave.y)
			|| threshold < abs(data.z - ave.z)
	  ){
		headAccelSum[0] = data.x; 
		headAccelSum[1] = data.y; 
		headAccelSum[2] = data.z;
		counter = 1;
		return true;
	}else{
		return false;
	}
}

vec3 headAve(vec3 data){
	headAccelSum[0] += data.x; 
	headAccelSum[1] += data.y; 
	headAccelSum[2] += data.z;
	counter++;
	vec3 result = {
		headAccelSum[0]/counter,
		headAccelSum[1]/counter,
		headAccelSum[2]/counter
		};
	return result;
}

vec3 lowPassFilter(vec3 pData,vec3 data){
	vec3 result = {
	(k * pData.x + (1.0 - k) * data.x),
	(k * pData.y + (1.0 - k) * data.y),
	(k * pData.z + (1.0 - k) * data.z)
	};
	return result;
}
