
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream> 
#include <string>  
#include <vector>  
#include <map>  
#include <unistd.h>

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h> 
#include <sstream>
using namespace std;
/**
*intתΪstring��
*/
string int_to_string(int num){
  ostringstream oss;
  string c = "";
  oss << num;
  c =oss.str();
  return c;
}

/**
@line_size:ÿ1���ж�����ĸ
@line:Ҫ��������
@count_thread:�ڼ�������
*/
int get_line_size(int line_size,int line,int count_thread){
	if(count_thread == 0){
		return 0;
	}else{
		return (line_size + 1) * (line * count_thread - 1) ;
	}
}

/*
��ÿ�ж�ȡ�ļ�¼����ϵͳ���ã���ͳ�Ƹü�¼���ļ��г��ֵ��к�,�������ݲμ�test����
*/
void test_grep(const char* in_file,const char* out_file,string filename,int thread_num,int line_size,int read_line_num,int is_end){
	fstream fout;
	fout.open(out_file,ios::out);
	
	ifstream fin;
	fin.open(in_file,ios::in);
	int count_read =0;
	fin.seekg(get_line_size(line_size,read_line_num,thread_num),std::ios::cur); 
	while(!fin.eof()){
		count_read++;
		string str_temp = "";
		getline(fin,str_temp);
		char tmp[1000];
		if(is_end == 0 && count_read > read_line_num){
				break;    //���ڲ������һ���ļ�������	
		}else if(str_temp.size() <= 2){
				break;	 //�������1���ļ�������
		}else{
			fout<<str_temp<<endl;
			string sql = "grep -n \""+str_temp+"\" "+filename;
			cout<<count_read<<endl;
			FILE *fp = popen(sql.c_str(), "r");   //ͨ��popen������ʵ��ϵͳ���ã�����1���ַ����飬�����ϵͳ���õ�����Ľ��
   			while (fscanf(fp, "%s", tmp) != EOF){
				for(int i = 0;i < 18;i++){
					if(tmp[i] != ':')fout<<tmp[i];
					else{
						fout<<endl;
						break;
					}
				}
			}
        			//puts(tmp);
			//fout<<tmp<<endl;
   			pclose(fp);
		}
	}

}


void test(const char* in_file,const char* out_file,int thread_num,int line_size,int read_line_num,int is_end){
	fstream fout;
	fout.open(out_file,ios::out);
	
	ifstream fin;
	fin.open(in_file,ios::in);
	int count_read =0;
	fin.seekg(get_line_size(line_size,read_line_num,thread_num),std::ios::cur); 
	while(!fin.eof()){
		count_read++;
		string str_temp = "";
		getline(fin,str_temp);
		if(is_end == 0 && count_read > read_line_num){
				break;
				
		}else if(str_temp.size() <= 2){
				break;
		}else{
			//cout<<count_read<<endl;
			fout<<str_temp<<endl;
		}
	}
}

int main(int argc,char *args[])
{
	char* file_1 = "/export/home/liaolanjie/xxq/hhh/BACKUP_RAW_DATA/carp_sequencing/female/assemblyreads/allfc/rightFc.txt";
	string file_name = "/export/home/liaolanjie/xxq/hhh/BACKUP_RAW_DATA/carp_sequencing/female/assemblyreads/carp-F-PCRfree_Round110_Lane8_1.fastq.raw_seq"; //Ҫ�޸�

	int  max_thread = 6;
	//file_1:�Ҷ��������ļ������޸�
	//file_name:rz�ļ�
	//max_thread:Ҫ�ܵĽ�����
	for(int i = 0;i <  max_thread;i++){
		if ( fork() == 0 ) {  
			/* �ӽ��̳��� */
 			//printf("This is child process  %d\n",i);
			int is_end = 0;    //�ж��Ƿ����ļ����1�飬0��ʾ���ǣ�1��ʾ��
			string out_file = "/export/home/liaolanjie/xxq/hhh/yc/Multi_Thread_linux/data/newtest/Round151_Lane1_1_" + int_to_string(i);     //out_file:���ɼ�¼�����λ��   ---Ҫ�޸�,��Ҫ�޸�MAKE�ļ�����3���ط��޸�
			if(i ==  max_thread -1)is_end = 1;
			//test(file_1,out_file.c_str(),i,76,4100000,is_end);
			test_grep(file_1,out_file.c_str(),file_name,i,30,100000,is_end);
			exit(0);   //���ڽ��̵��˳�
		}
		else {
		
 			printf("This is process process\n");
		} 
	}
	return 0;
}

