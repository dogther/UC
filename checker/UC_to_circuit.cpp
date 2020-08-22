#include<cstdio>
#define maxn 100010
char tmp[maxn];
int inputs[maxn],outputs[maxn],n_input=0,n_output=0;
int trans[maxn],end_sig[maxn];
int typ[maxn],in1[maxn],in2[maxn],ou1[maxn];
int _typ[maxn],_num[maxn][4];
int n_gates,n_wires,tot_gates;
#define ADD 1
#define SUB 6
#define MULT 2
void add_gate(int ty,int i1,int i2,int o1)
{
	typ[tot_gates]=ty;
	in1[tot_gates]=i1;
	in2[tot_gates]=i2;
	ou1[tot_gates]=o1;
	tot_gates++;
}
int main(int argc,char **argv)
{
	FILE *cir = fopen(argv[1],"r");
	FILE *res = fopen(argv[2],"w");
	//FILE *cir = fopen("circuit_uc.txt","r");
	//FILE *res = fopen("circuit_input.txt","w");
	char s[15];
	int n_parties = 3;
	fscanf(cir,"%s",s);
	int max_wire=0;
	bool ouf=true;
	while(ouf)
	{
		if(s[0]=='C')
		{
			while(fscanf(cir,"%s",s)!=EOF)
			{
				if(s[0]>='0' && s[0]<='9')
				{
					sscanf(s,"%d",inputs+n_input);
					if(inputs[n_input]>max_wire) max_wire=inputs[n_input];
					n_input++;
				}
				else break;
			}
		}
		else if(s[0]=='O')
		{
			while(fscanf(cir,"%s",s)!=EOF)
			{
				if(s[0]>='0' && s[0]<='9')
				{
					sscanf(s,"%d",outputs+n_output);
					n_output++;
				}
				else {ouf=false; break;}
			}
			ouf=false; break;
		}
		else if(s[0]=='X') //x gate
		{
			_typ[n_gates] = 1;
			for(int i=0; i<4; i++)
			{
				fscanf(cir,"%d",&_num[n_gates][i]);
				if(_num[n_gates][i]>max_wire) max_wire = _num[n_gates][i];
			}
			n_gates++;
			if(fscanf(cir,"%s",s)==EOF) break;
		}
		else if(s[0]=='Y' || s[0]=='U') 
		{
			if(s[0]=='Y') _typ[n_gates] = 2;
			else _typ[n_gates] = 3;
			for(int i=0; i<3; i++)
			{
				fscanf(cir,"%d",&_num[n_gates][i]);
				if(_num[n_gates][i]>max_wire) max_wire = _num[n_gates][i];
			}
			n_gates++;
			if(fscanf(cir,"%s",s)==EOF) break;
		}
		else
		{
			printf("Impossible!");
			return 0;
		}
	}
	//deal with each gate
	tot_gates = 0;
	for(int i=0; i<n_gates; i++)
	{
		//distribute input for code
		int _code = ++max_wire;
		inputs[n_input++]=_code;
		if(_typ[i]==1) //X gate
		{
			//tmp1 = p * a
			max_wire++;
			add_gate(MULT,_num[i][0],_code,max_wire);
			//tmp2 = p * b
			max_wire++;
			add_gate(MULT,_num[i][1],_code,max_wire);
			//out1 = a - (p*a) + (p*b)
			//out2 = b + (p*a) - (p*b)
			max_wire++;
			add_gate(SUB,max_wire-1,max_wire-2,max_wire);
			add_gate(ADD,_num[i][0],max_wire,_num[i][2]);
			add_gate(SUB,_num[i][1],max_wire,_num[i][3]);
		}
		else if(_typ[i]==2) //Y gate
		{
			//out1 = p*a + (1-p)*b
			max_wire++;
			add_gate(MULT,_num[i][0],_code,max_wire);
			max_wire++;
			add_gate(MULT,_num[i][1],_code,max_wire);
			max_wire++;
			add_gate(SUB,_num[i][1],max_wire-1,max_wire);
			add_gate(ADD,max_wire-2,max_wire,_num[i][2]);
		}
		else if(_typ[i]==3) //U gate
		{
			//first add 3 extra inputs
			inputs[n_input++] = _code+1;
			inputs[n_input++] = _code+2;
			inputs[n_input++] = _code+3;
			max_wire+=3;
			//4 function bits
			//out1 = p0 * a * b + p1 * a + p2 * b + p3
			//UNUSED out1 = (1-p) * (a+b) + p * a * b
			max_wire++;
			add_gate(MULT,_code,_num[i][0],max_wire);
			max_wire++;
			add_gate(MULT,max_wire-1,_num[i][1],max_wire);
			max_wire++;
			add_gate(MULT,_code+1,_num[i][0],max_wire);
			max_wire++;
			add_gate(MULT,_code+2,_num[i][1],max_wire);
			max_wire++;
			add_gate(ADD,max_wire-3,max_wire-2,max_wire);
			max_wire++;
			add_gate(ADD,max_wire-2,_code+3,max_wire);
			add_gate(ADD,max_wire-1,max_wire,_num[i][2]);
			/*
			max_wire++;
			add_gate(ADD,max_wire-2,max_wire-1,max_wire);
			max_wire++;
			add_gate(ADD,_num[i][0],_num[i][1],max_wire);
			max_wire++;
			add_gate(MULT,_code,max_wire-1,max_wire);
			max_wire++;
			add_gate(SUB,max_wire-2,max_wire-1,max_wire);
			max_wire++;
			add_gate(MULT,_code,_num[i][0],max_wire);
			max_wire++;
			add_gate(MULT,max_wire-1,_num[i][1],max_wire);
			add_gate(ADD,max_wire-2,max_wire,_num[i][2]);
			*/
		}
	}
	//swap the wire of input and other gates
	for(int i=0; i<=max_wire; i++) trans[i]=i,end_sig[i]=i;
	for(int i=0; i<n_input; i++)
	{
		//int t = trans[i];
		trans[inputs[i]] = i;
		trans[end_sig[i]] = inputs[i];
		end_sig[inputs[i]] =  end_sig[i];
		//trans[t] = t;
		//trans[t] = trans[i];
		//trans[i] = t;
		//trans[inputs[i]] = i;
		//trans[i] = t;
	}
	//output the gates
	fprintf(res,"%d\n%d\n",tot_gates,n_parties);
	fprintf(res,"1 %d",n_input);
	for(int i=0; i<n_input; i++)
		fprintf(res," %d",trans[inputs[i]]);
	fprintf(res,"\n");
	for(int i=2; i<=n_parties; i++)
		fprintf(res,"%d 0\n",i);
	fprintf(res,"1 %d",n_output);
	for(int i=0; i<n_output; i++)
		fprintf(res," %d",trans[outputs[i]]);
	fprintf(res,"\n");
	for(int i=2; i<=n_parties; i++)
		fprintf(res,"%d 0\n",i);
	for(int i=0; i<tot_gates; i++)
		fprintf(res,"2 1 %d %d %d %d\n",trans[in1[i]],trans[in2[i]],trans[ou1[i]],typ[i]);
	fclose(res);
	fclose(cir);
	return 0;
}
