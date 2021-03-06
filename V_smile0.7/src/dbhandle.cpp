#include "../include/dbhandle.h"

#define maxdscrb 128

struct RECORD records[20];

/***********************  start about database  ******************************/
struct USER *dblogin(char name[32], char passwd[24])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	struct USER *lginres;
	//lginres = (struct USER *)malloc(sizeof(struct USER *));
	lginres = new USER();
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		lginres->Islginsuccess = 0;
		return lginres;
	}

	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		lginres->Islginsuccess = 0;
		return lginres;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		lginres->Islginsuccess = 0;
		return lginres;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		cout<<row[1]<<"  "<<name<<endl;
		pname = strcmp(row[1], name);
		if(pname == 0)
		{
			//int ppasswd;
			if(!strcmp(row[6], passwd))
			{
				cout << "login success!" << endl;				
				//passwd correct
				lginres->Islginsuccess = 1;
				lginres->username = row[1];
				lginres->sex = atoi(row[2]);
				//cout << row[2] << endl;
				//cout << *row[2] <<endl;
				lginres->age = atoi(row[3]);
				//cout << lginres->age << endl;
				lginres->facetype = atoi(row[4]);
				//cout << lginres->facetype << endl;
				lginres->mail = row[5];
				lginres->portrait = atoi(row[7]);
				//cout << lginres->portrait << endl;
				
				return lginres;
			}
			else
			{
				cout << "passwd error, login failure!" << endl;
				//passwd error
				lginres->Islginsuccess = 2;
				return lginres;
			}
			
		}
		//break;
	}
	
	//free(lginres);
	delete lginres;
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int registration(struct USER *pusrinfor)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	/********get the information of the user *********/
	string pwd, name, mail;
	int sex, age, facetype, portrt;
	name = pusrinfor->username;
	pwd = pusrinfor->passwd;
	mail = pusrinfor->mail;
	sex = pusrinfor->sex;
	age = pusrinfor->age;
	facetype = pusrinfor->facetype;
	portrt =pusrinfor->portrait;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select username from users", (unsigned long)strlen("select username from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return 0;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[0], name.c_str());
		if(pname == 0)
		{
			cout << "this username exist!" << endl;
			return 2;
		}
		//break;
	}
	
	stringstream ss1, ss2, ss3, ss4;
	ss1<<sex; ss2<<age; ss3<<facetype; ss4<<portrt;
	string ssex = ss1.str();
	string sage = ss2.str();
	string sfacetype = ss3.str();
	string sportrt = ss4.str();
	
	string comd = "insert into users (username,sex,age,facetype,mail,passwd,portrait) values('"+name+"','"+ssex+"','"+sage+"','"+sfacetype+"','"+mail+"','"+pwd+"','"+sportrt+"')";
//	string comd = "insert into users (username, mail, passwd)"
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "register success!" << endl;
		return 1;
	}

	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int pwdmodify(char name[16], char newpwd[24])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return 0;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[1], name);
		if(pname == 0)
		{
			string spwd, sname;
			spwd = newpwd; 
			sname = name;
			string comd = "update users set password='"+spwd+"' where username='"+sname+"'";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				
				cout << "mysql_query failure!" << endl;
				return 0;
			}
			else
			{
				cout << "passwd modify success!" << endl;
				return 1;
			}
			//row[9] = newpwd;
			//cout << "psd modify success!" << endl;
			//return 1;	
		}
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int changeinfo(struct USER *pchginfor)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	string pwd, name, mail;
	int sex, age, facetype, portrt;
	name = pchginfor->username;
	pwd = pchginfor->passwd;
	mail = pchginfor->mail;
	sex = pchginfor->sex;
	age = pchginfor->age;
	facetype = pchginfor->facetype;
	portrt = pchginfor->portrait;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return 0;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[1], name.c_str());
		if(pname == 0)
		{
			/*string spwd, sname;
			spwd = newpwd; 
			sname = name;*/

			stringstream ss1, ss2, ss3, ss4;
			ss1<<sex; ss2<<age; ss3<<facetype; ss4<<portrt;
			string ssex = ss1.str();
			string sage = ss2.str();
			string sfacetype = ss3.str();
			string sportrt = ss4.str();

			string comd = "update users set mail='"+mail+"',facetype='"+sfacetype+"',age='"+sage+"',sex='"+ssex+"',password='"+pwd+"',portrait='"+sportrt+"' where username='"+name+"'";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				
				cout << "change information failure!" << endl;
				return 0;
			}
			else
			{
				cout << "information modify success!" << endl;
				return 1;
			}
			//row[9] = newpwd;
			//cout << "psd modify success!" << endl;
			//return 1;	
		}
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

struct qunlist *publicqun(void)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	struct qunlist *p_qunlist;
	//p_qunlist = (struct qunlist *)malloc(sizeof(struct qunlist));
	p_qunlist = new qunlist();
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}

	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
		
	int i = 0;
	while(row = mysql_fetch_row(res))
	{	
		struct quninfor *p_quninfor;
		//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
		p_quninfor = new quninfor();
		p_quninfor = &(p_qunlist->qungroup[i]);
		p_quninfor->qunportrait = atoi(row[3]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
		p_quninfor->qunname = row[1];
		p_quninfor->qundescrible = row[4];

		++i;
		if(i == 10)
		{
			//free(p_quninfor);
			delete p_quninfor;
			return p_qunlist;
			//break;
		}
	}
				
	//free(p_qunlist);
	delete p_qunlist;
	mysql_free_result(res);
	mysql_close(&mysql);

}

struct qunlist *privatequn(string username)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;

	struct qunlist *p_qunlist; 	//the maximun number of privatequn is 10
	//p_qunlist = (struct qunlist *)malloc(sizeof(struct qunlist));
	p_qunlist = new qunlist();
	struct quninfor *qungroup = new quninfor[5];
	p_qunlist->qungroup = qungroup; 
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}

	if(mysql_real_query(&mysql, "select * from user_qun", (unsigned long)strlen("select * from user_qun")))
	{
		cout << "mysql_real_query failure!" <<endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
		
	//int i = 0;
	while(row = mysql_fetch_row(res))
	{
		int pusrname;
		pusrname = strcmp(username.c_str(), row[1]);
		if(pusrname == 0)
		{
			string qunname = row[2];
			if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
			{
				cout << "mysql_real_query failure!" <<endl;
				p_qunlist->Issuccess = 0;
				return p_qunlist;
			}
			
			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result failure!" << endl;
				p_qunlist->Issuccess = 0;
				return p_qunlist;
			}
			
			int i = 0;
			while(row2 = mysql_fetch_row(res2))
			{
				int pqunname;
				pqunname = strcmp(qunname.c_str(), row2[1]);
				if(pqunname == 0)
				{
					struct quninfor *p_quninfor;
					//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
					p_quninfor = new quninfor();
					p_quninfor = &(p_qunlist->qungroup[i]);
					p_quninfor->qunportrait = atoi(row2[3]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
					p_quninfor->qunname = row2[1];
					p_quninfor->qundescrible = row2[4];

					++i;

					//free(p_quninfor);
					delete p_quninfor;
				}
				p_qunlist->Issuccess = i; 	//store the number of the pivatequn.
			}
			
			return p_qunlist;
		}
		
	}
				
	//free(p_qunlist);
	delete p_qunlist;
	delete[] qungroup;
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);

}

struct quninfor *joinqun(string username, string qunname)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	struct quninfor *p_quninfor;
	//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
	p_quninfor = new quninfor();
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return p_quninfor;
	}

	/*if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return;
	}*/

	/*string squnname, suser;
	squnname = qunname; 
	suser = username;*/
	string comd = "insert into user_qun (username, qunname) values('"+username+"','"+qunname+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return p_quninfor;
	}
	else
	{
		cout << "join in the qun success!" << endl;
		if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
		{
			cout << "mysql_real_query failure!" <<endl;
			return p_quninfor;
		}
	
		res = mysql_store_result(&mysql);
		if(res == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			return p_quninfor;
		}
		
		while(row = mysql_fetch_row(res))
		{
			int pqunname;
			pqunname = strcmp(qunname.c_str(), row[1]);
			if(pqunname == 0)
			{
				p_quninfor->qunportrait = atoi(row[3]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
				p_quninfor->qunname = row[1];
				p_quninfor->qundescrible = row[4];
					
				return p_quninfor;
			}
				
		}
		//return 1;
	}
	
	//free(p_quninfor);
	delete p_quninfor;
	mysql_free_result(res);
	mysql_close(&mysql);

}

struct quninfor *searchqun(string qunname)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	struct quninfor *p_quninfor;
	//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
	p_quninfor = new quninfor();
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return p_quninfor;
	}

	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return p_quninfor;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return p_quninfor;
	}
		
	while(row = mysql_fetch_row(res))
	{
		int pqunname;
		pqunname = strcmp(qunname.c_str(), row[1]);
		if(pqunname == 0)
		{
			p_quninfor->qunportrait = atoi(row[3]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
			p_quninfor->qunname = row[1];
			p_quninfor->qundescrible = row[4];
					
			return p_quninfor;
		}
				
	}

	//free(p_quninfor);
	delete p_quninfor;
	mysql_free_result(res);
	mysql_close(&mysql);

}

int createqun(string qunname, string qunmaster, int qunportrt, string qundscrb)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select qunname from qun_tab", (unsigned long)strlen("select qunname from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return 0;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[0], qunname.c_str());
		if(pname == 0)
		{
			cout << "this qun name exist!" << endl;
			return 2;
		}
		//break;
	}

	/*string smaster, sname, sdscrb;
	smaster = qunmaster; 
	sname = qunname;
	sdscrb = qundscrb;
	string sportrt = qunportrt;*/
	stringstream ss;
	ss<<qunportrt;
	string sportrt = ss.str();
	string comd = "insert into qun_tab (qunname,qunmaster, qunportrait, qundescrible) values('"+qunname+"','"+qunmaster+"','"+sportrt+"','"+qundscrb+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "create qun success!" << endl;
		return 1;
	}

	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

struct RECORD *showchatrcd(string username, string qunname)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2, *res3;
	MYSQL_ROW row, row2, row3;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return;
	}

	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname;
		char qunname[32];
		string recordno;
		char *qunID;
		qunID = (char *)malloc(4*sizeof(char)); 
		pname = strcmp(row[1], qunname);
		if(pname == 0)
		{
			qunID = row[0];
			switch((int)(*qunID)){
				case '1':
					recordno = "qun1_record";
					break;
				case '2':
					recordno = "qun2_record";
					break;
				case '3':
					recordno = "qun3_record";
					break;
				case '4':
					recordno = "qun4_record";
					break;
				case '5':
					recordno = "qun5_record";
					break;
				case '6':
					recordno = "qun6_record";
					break;
				case '7':
					recordno = "qun7_record";
					break;
				case '8':
					recordno = "qun8_record";
					break;
				case '9':
					recordno = "qun9_record";
					break;
				/*case '10':
					recordno = "qun10_record";
					break;*/
				default:
					cout << "exceed the maximun numberof the qun, error!" << endl;
			}
			
			string squnno;
			squnno = recordno;
			//string comd = "insert into '"+squnno+"' (qunname, username, cur_time, arecord) values('"+squnname+"','"+suser+"', '"+stime+"','"+srecd+"')";
			string comd = "select * from '"+squnno+"'";
			cout << comd.c_str() << endl;
			if(mysql_real_query(&mysql, comd.c_str(), (unsigned long)strlen(comd.c_str())))
			{
				cout << "mysql_real_query failure!" <<endl;
				return 0;
			}
			else
			{
				cout << "query chat record success!" << endl;
			}	

			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result failure!" << endl;
				return 0;
			}
			
			int i = 0;
			while(row2 = mysql_fetch_row(res2))
			{
				/*(record[i]).username = row[1];
				(record[i]).cur_time = row[2];
				(record[i]).message = row[3];*/

				/*char *usrname, *curtime, *msg;
				usrname = (char *)malloc(32*sizeof(char));
				curtime = (char *)malloc(32*sizeof(char));
				msg = (char *)malloc(maxmsglen*sizeof(char));
				usrname = row[1];
				curtime = row[2];
				msg = row[3];
				(record[i]).username = usrname;
				(record[i]).cur_time = curtime;
				(record[i]).message = msg;*/
				
				struct RECORD *p_rcd;
				//p_rcd = (struct RECORD *)malloc(sizeof(struct RECORD));
				p_rcd = new RECORD();
				p_rcd = &(records[i]);
				//string row1 = row[1];

				p_rcd->username = row2[1];
				p_rcd->cur_time = row2[2];
				p_rcd->message = row2[3];
				
				string speaker;
				speaker = p_rcd->username;
				if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
				{
					cout << "mysql_real_query failure!" <<endl;
					//return;
				}
	
				res3 = mysql_store_result(&mysql);
				if(res3 == NULL)
				{
					cout << "mysql_store_result failure!" << endl;
					//return;
				}

				while(row3 = mysql_fetch_row(res3))
				{
					int pname;
					pname = strcmp(speaker.c_str(), row3[1]);
					if(pname == 0)
					{
						p_rcd->usrportrt = atoi(row3[7]);
						break;
					}
				}
				
				++i;
				if(i == 20)
				{
					/*free(usrname);
					free(curtime);
					free(msg);*/
					break;
				}
				delete p_rcd;
			}	
		}
		
	}
	
	//free(precord);	
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_free_result(res3);
	mysql_close(&mysql);

	return records;
}

char *gettime(void)
{
	time_t now;
	//char *ctime();

	(void) time(&now);
	return ctime(&now);
}

int addchatrecord(string username, string qunname, string msg)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return 0;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname;
		//char qunID[4];
		//char qunname[32];
		//char recordno[12];
		string recordno;  
		char *qunID;
		qunID = (char *)malloc(4*sizeof(char));
		pname = strcmp(row[1], qunname.c_str());
		if(pname == 0)
		{
			qunID = row[0];
			switch((int)(*qunID)){
				case '1':
					recordno = "qun1_record";
					break;
				case '2':
					recordno = "qun2_record";
					break;
				case '3':
					recordno = "qun3_record";
					break;
				case '4':
					recordno = "qun4_record";
					break;
				case '5':
					recordno = "qun5_record";
					break;
				case '6':
					recordno = "qun6_record";
					break;
				case '7':
					recordno = "qun7_record";
					break;
				case '8':
					recordno = "qun8_record";
					break;
				case '9':
					recordno = "qun9_record";
					break;
				/*case '10':
					recordno = "qun10_record";
					break;*/
				default:
					cout << "exceed the maximun numberof the qun, error!" << endl;
			}
			
			/*string squnname, suser, squnno, stime, srecd;
			squnname = qunname; 
			suser = username;
			squnno = recordno;*/
			string stime;
			stime = gettime();
			//srecd = record;

			
			string comd = "insert into '"+recordno+"' (qunname, username, cur_time, arecord) values('"+qunname+"','"+username+"', '"+stime+"','"+msg+"')";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				cout << "mysql_query failure!" <<endl;
				return 0;
			}
			else
			{
				cout << "add chat record success!" << endl;
				return 1;
			}	
			
		}
		free(qunID);
		//break;
	}
	
		
	mysql_free_result(res);
	mysql_close(&mysql);

}

int insert_hottie(int picID, string author, string title, string text)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}
	
	stringstream ss;
	ss<<picID; 
	string spicID = ss.str();

	string comd = "insert into hottie (author, picture, title, text) values('"+author+"','"+spicID+"','"+title+"','"+text+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "insert hot tie failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "insert hot tie success!" << endl;
		return 1;
	}

	mysql_free_result(res);
	mysql_close(&mysql);
	
}

struct hottie *send_hottie(int times)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	struct hottie *p_hottie;
	//p_hottie = (struct hottie *)malloc(sizeof(struct hottie));
	p_hottie = new hottie();
	struct hottie *hotties;
	hotties = new hottie[5];
	//hotties = (struct hottie *)malloc(5*sizeof(struct hottie));
	//struct hottie hotties[5];
	//hotties = p_hotties;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsimle", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return 0;
	}
	
	if(mysql_real_query(&mysql, "select * from hottie", (unsigned long)strlen("select * from hottie")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return;
	}

	int i = 0;
	while(row = mysql_fetch_row(res))
	{
		p_hottie = &(hotties[i]);
		p_hottie->picture = atoi(row[2]);
		p_hottie->author = row[1];
		p_hottie->title = row[3];
		p_hottie->text = row[4];
		
		++i;
		if(i == times)
			break;
	}
	//free(p_hottie);
	delete p_hottie;
	return hotties;
	
	//free(p_hottie);
	//free(hotties);
	delete[] hotties;
	
	mysql_free_result(res);
	mysql_close(&mysql);
	
}

/*************************  end about database  ************************************/
