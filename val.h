#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>
#include <cmath>

using namespace std;

enum ValType { VINT, VREAL, VSTRING, VBOOL, VERR };

class Value {
    ValType	T;
    bool    Btemp;
	int		Itemp;
	float   Rtemp;
    string	Stemp;
       
public:
    Value() : T(VERR), Btemp(false), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(bool vb) : T(VBOOL), Btemp(vb), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(int vi) : T(VINT), Btemp(false), Itemp(vi), Rtemp(0.0), Stemp("") {}
	Value(float vr) : T(VREAL), Btemp(false), Itemp(0), Rtemp(vr), Stemp("") {}
    Value(string vs) : T(VSTRING), Btemp(false), Itemp(0), Rtemp(0.0), Stemp(vs) {}
    
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsString() const { return T == VSTRING; }
    bool IsReal() const {return T == VREAL;}
    bool IsBool() const {return T == VBOOL;}
    
    int GetInt() const { if( IsInt() ) return Itemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    string GetString() const { if( IsString() ) return Stemp; throw "RUNTIME ERROR: Value not a string"; }
    
    float GetReal() const { if( IsReal() ) return Rtemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    bool GetBool() const {if(IsBool()) return Btemp; throw "RUNTIME ERROR: Value not a boolean";}
    
    void SetType(ValType type)
    {
    	T = type;
	}
	
    void SetInt(int val)
    {
    	Itemp = val;
	}
	
	void SetReal(float val)
    {
    	Rtemp = val;
	}
	
	void SetString(string val)
    {
    	Stemp = val;
	}
	
	void SetBool(bool val)
    {
    	Btemp = val;
	}
	
	
    // add op to this
     Value operator+(const Value& op) const{
       Value RESULT;
       if(this->GetType()==VERR || op.GetType()==VERR || op.GetType()==VBOOL || this->GetType()==VBOOL){
           RESULT.SetType(VERR);   
       }

       else if(this->IsString() && op.IsString()){
           RESULT.SetType(VSTRING);
           string stringres = this->GetString() + op.GetString();
           //cout<<"STRING ADDITION IS: "<<stringres<<"|";
           RESULT.SetString(stringres);
       }


       else if(this->IsInt() && op.IsInt()){
           RESULT.SetType(VINT);
           int tempint = this->GetInt() + op.GetInt();
           RESULT.SetInt(tempint);
       }
    

       else if(this->IsReal() && op.IsInt()){

            RESULT.SetType(VREAL);
                    
            float tempfloat = this->GetReal() + op.GetInt();

            RESULT.SetReal(tempfloat);

       }
       else if(this->IsInt() && op.IsReal()){

            RESULT.SetType(VREAL);
                    
            float tempfloat = this->GetInt() + op.GetReal();

            RESULT.SetReal(tempfloat);
       }
       else if(this->IsReal() && op.IsReal()){

            RESULT.SetType(VREAL);
                    
            float tempfloat = this->GetReal() + op.GetReal();

            RESULT.SetReal(tempfloat);
       }
       else{
           RESULT.SetType(VERR);
       }
       //cout<<"RETURNING! THE TYPE OF RESULT IS: "<<RESULT.GetType()<<endl;
       return RESULT;

    } 

     // multiply this by op
    Value operator*(const Value& op) const{
        Value RESULT;

        if((this->IsReal()||this->IsInt()) && (op.IsReal()||op.IsInt())){
            
            RESULT.SetType(VREAL);
            float firstNum = 0;
            float secondNum= 0;

            if(this->IsReal()){
                firstNum = this->GetReal();
            }
            if(this->IsInt()){
                firstNum = this->GetInt();

            }

            if(op.IsReal()){
                secondNum = op.GetReal();

            }
            if(op.IsInt()){
                secondNum = op.GetInt();
            }

            RESULT.SetReal(firstNum*secondNum);

        }   
    
        else{
            RESULT.SetType(VERR);
        }
        
        return RESULT;

    };

    

     Value operator==(const Value& op) const{
        Value RESULT;

        if(this->GetType()==op.GetType()){

            if(this->GetType()==VSTRING && op.GetType()==VSTRING){
                
                RESULT.SetType(VBOOL);
                if(this->GetString()==op.GetString()){
                    RESULT.SetBool(true);
                }else{
                    RESULT.SetBool(false);
                }
            }


            else if(this->GetType()==VINT && op.GetType()==VINT){
                
                RESULT.SetType(VBOOL);
                if(this->GetInt()==op.GetInt()){
                    RESULT.SetBool(true);
                }else{
                    RESULT.SetBool(false);
                }
            }

            else if(this->GetType()==VREAL && op.GetType()==VREAL){
                
                RESULT.SetType(VBOOL);
                if(this->GetReal()==op.GetReal()){
                    RESULT.SetBool(true);
                }else{
                    RESULT.SetBool(false);
                }
            }
            else{
                RESULT.SetType(VERR);
            }


        }
        else if(this->GetType() != op.GetType()){

            if(this->GetType()==VINT && op.GetType()==VREAL){
                
                RESULT.SetType(VBOOL);
                if(this->GetInt()==op.GetReal()){
                    RESULT.SetBool(true);
                }else{
                    RESULT.SetBool(false);
                }
            }


            else if(this->GetType()==VREAL && op.GetType()==VINT){
                
                RESULT.SetType(VBOOL);
                if(this->GetReal()==op.GetInt()){
                    RESULT.SetBool(true);
                }else{
                    RESULT.SetBool(false);
                }
            }
            else{
                RESULT.SetType(VERR);
            }

        }else{
            if(this->GetType()!=op.GetType()){
                cout<<"bruh"<<this->GetType()<<endl;
                RESULT.SetType(VERR);
            }
            
        }


        return RESULT;
        
    }


    // subtract op from this
    Value operator-(const Value& op) const{
        Value RESULT;
       if(this->GetType()==VERR || op.GetType()==VERR || op.GetType()==VBOOL || this->GetType()==VBOOL){
           RESULT.SetType(VERR);   
       }

       else if(this->IsInt() && op.IsInt()){
           RESULT.SetType(VINT);
           int tempint = this->GetInt() - op.GetInt();
           RESULT.SetInt(tempint);
       }
    

       else if(this->IsReal() && op.IsInt()){

            RESULT.SetType(VREAL);
                    
            float tempfloat = this->GetReal() - op.GetInt();

            RESULT.SetReal(tempfloat);

       }
       else if(this->IsInt() && op.IsReal()){

            RESULT.SetType(VREAL);
                    
            float tempfloat = this->GetInt() - op.GetReal();

            RESULT.SetReal(tempfloat);
       }
       else if(this->IsReal() && op.IsReal()){

            RESULT.SetType(VREAL);
                    
            float tempfloat = this->GetReal() - op.GetReal();

            RESULT.SetReal(tempfloat);
       }
       else{
           RESULT.SetType(VERR);
       }
       //cout<<"RETURNING! THE TYPE OF RESULT IS: "<<RESULT.GetType()<<endl;
       return RESULT;

    
    }
    
    
    
    // divide this by op
    Value operator/(const Value& op) const{

        Value RESULT;

        if((this->IsReal()||this->IsInt()) && (op.IsReal()||op.IsInt())){
            
            RESULT.SetType(VREAL);
            float firstNum = 0;
            float secondNum= 0;

            if(this->IsReal()){
                firstNum = this->GetReal();
            }
            if(this->IsInt()){
                firstNum = this->GetInt();

            }

            if(op.IsReal()){
                secondNum = op.GetReal();

            }
            if(op.IsInt()){
                secondNum = op.GetInt();
            }

            RESULT.SetReal(firstNum/secondNum);
            
            if(secondNum==0){
                RESULT.SetType(VERR);
            }
        }   
    
        else{
            RESULT.SetType(VERR);
        }
        
        return RESULT;
    }
    
	Value operator>(const Value& op) const{
        Value RESULT;
        RESULT.SetType(VBOOL);

        if((this->IsReal() || this->IsInt()) && (op.IsReal() || op.IsInt())){
    
            if(this->IsReal()){

                if(op.IsReal()){
                    if(this->GetReal()>op.GetReal()){
                        RESULT.SetBool(true);
                    }else{
                        RESULT.SetBool(false);
                    }
                }
                if(op.IsInt()){
                    if(this->GetReal()>op.GetInt()){
                        RESULT.SetBool(true);
                    }else{
                        RESULT.SetBool(false);
                    }
                }

            }

            if(this->IsInt()){
                if(op.IsReal()){
                    if(this->GetInt()>op.GetReal()){
                        RESULT.SetBool(true);
                    }else{
                        RESULT.SetBool(false);
                    }
                }
                if(op.IsInt()){
                    if(this->GetInt()>op.GetInt()){
                        RESULT.SetBool(true);
                    }else{
                        RESULT.SetBool(false);
                    }
                }
            }
        }else{
            RESULT.SetBool(false);
        }
        return RESULT;
    }
	
	Value operator<(const Value& op) const{
        Value RESULT;
        RESULT.SetType(VBOOL);

        if((this->IsReal() || this->IsInt()) && (op.IsReal() || op.IsInt())){
    
            if(this->IsReal()){

                if(op.IsReal()){
                    if(this->GetReal()<op.GetReal()){
                        RESULT.SetBool(true);
                    }else{
                        RESULT.SetBool(false);
                    }
                }
                if(op.IsInt()){
                    if(this->GetReal()<op.GetInt()){
                        RESULT.SetBool(true);
                    }else{
                        RESULT.SetBool(false);
                    }
                }

            }

            if(this->IsInt()){
                if(op.IsReal()){
                    if(this->GetInt()<op.GetReal()){
                        RESULT.SetBool(true);
                    }else{
                        RESULT.SetBool(false);
                    }
                }
                if(op.IsInt()){
                    if(this->GetInt()<op.GetInt()){
                        RESULT.SetBool(true);
                    }else{
                        RESULT.SetBool(false);
                    }
                }
            }
        }else{
            RESULT.SetBool(false);
        }
        return RESULT;
    }
	
	    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.Itemp;
        else if( op.IsString() ) out << op.Stemp;
        else if( op.IsReal()) out <<  fixed << showpoint << setprecision(2) << op.Rtemp;
        else out << "ERROR";
        return out;
    }
};


#endif
