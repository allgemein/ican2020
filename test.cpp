#include<bits/stdc++.h>
typedef struct vec{
	float x;
	float y;
	float z;
	vec operator+(const vec& v){
		vec ret;
		ret.x = x + v.x;
		ret.y = y + v.y;
		ret.z = z + v.z;
		return ret;
	}
	vec operator-(const vec& v){
		vec ret;
		ret.x = x - v.x;
		ret.y = y - v.y;
		ret.z = z - v.z;
		return ret;
	}
	vec operator*(const double d){
		vec ret;
		ret.x = d * x;
		ret.y = d * y;
		ret.z = d * z;
		return ret;
	}
	vec& operator+=(const vec& v){
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	vec& operator-=(const vec& v){
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
} vec3;
using namespace std;
int main(){
	vec3 v1,v2;
	cin>>v1.x>>v1.y>>v1.z>>v2.x>>v2.y>>v2.z;
	vec3 sum=v1+v2;
	cout<<sum.x<<","<<sum.y<<","<<sum.z<<endl;
}
