template <typename T>
class lowPassFillter {
  private:
    float k;
  protected:
    T data, pData;
  public:
    lowPassFillter<T>(float k) {
      this->k = k;
    }
    void setData(T data) {
      this->data = data;
    }
	virtual T getData() {
		return data;
	}
	virtual T getResult(){
		return pData;
	}
    T LPF() {
      T result = (k * pData + (1.0 - k) * data);
      pData = result;
      return result;
    }
};


template <typename T>
class highPassFillter: public lowPassFillter<T> {
  public:
  	highPassFillter<T>(float k):lowPassFillter<T>(k){
		}
    T HPF() {
      T result = abs(this->LPF() - this->data);
      return result;
    }
};

template <typename T>
class bandPassFillter:public highPassFillter<T>{
 private:
   T dataB,pDataB;
   float kB;
 public:
   bandPassFillter<T>(float k,float kB):highPassFillter<T>(k){
     this->kB=kB;
   }
   T getData(){
	   return dataB;
   }
   T getResult(){
	   return pDataB;
   }
   T BPF(){
     dataB=this->HPF();
     T result = (kB*pDataB+(1.0-kB)*dataB);
     pDataB = result;
     return result;
   }
};
