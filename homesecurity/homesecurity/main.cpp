
//  main.cpp
//  homesecurity
//
//  Created by Nicholas Bouckaert on 9/30/18.
//  Copyright © 2018 Nicholas Bouckaert. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include <iomanip>
#include <pthread.h>
#define ACCOUNTS "accounts.txt"

//Uncomment out only when compiling on the pi will not work on xcode
#include <wiringPi.h>

//all defines will be references as below in the code
#define vibration_pin 15
#define motion_pin 16
#define speaker_pin 1 
#define H HIGH
#define L LOW

void *set_up_sensors( void* event_detected );
void *sensors_check_thread( void* event_detected);


int event_detected = 0;
int alarm_flag = 0;

using namespace std;

class Log_in
{
    public:
        Log_in(){};
        ~Log_in(){};
        void login();
        void locate_account( string id, string password);
    	void change_alarm_status(string pin);
    
    
    protected:
        string user_id;
        string user_password;
        int user_access_level;
	string user_pin; 
    
};

class User_Account: public Log_in
{
    public:
        User_Account(string user_id, string user_password, string user_pin);
        ~User_Account(){};
         void display_user_menu();
	
  private: 
	string account_id;
	string account_password;
	string account_pin;
    
    
};

class Admin_Account: public Log_in
{
    public:
            Admin_Account(){display_admin_menu();};
            ~Admin_Account(){};
            void display_admin_menu();
    
};
/*
****************************************************************************************
			    INT MAIN
****************************************************************************************
*/

int main(int argc, const char * argv[]) {
    // insert code here...
    	// Was only used for testing purposes 
	//pthread_t initialize_sensors; 
	//pthread_create( &initialize_sensors, NULL, &set_up_sensors, NULL);
	//pthread_detach( initialize_sensors);
	Log_in user;
    	user.login();
    
    return 0;
}


/*
 ***************************************************************************************
                            LOG_IN CLASS BEGINS HERE
 ***************************************************************************************
*/
void Log_in:: login()
{
    string input_id, input_password, exit_program = "Still in program", space;
    do
    {
        try
            {
                cout<<"***********Welcome to Security Plus*********"<<endl;
                cout<<"Please enter in your id: "<<endl;
                getline( cin, input_id);
            
                cout<<"Please enter in your password: "<<endl;
                getline( cin, input_password);
            
                locate_account( input_id, input_password);
            }
        
        catch( int no_account_found)//Account was not found in locate account
            {
                for( int i = 0; i < 1000; i++)
                {
                    cout<<endl;
                }
                cout<<"not account found"<<endl;
                //get the space
                //getline(cin, space);
                
            }
        catch(...)//default catch
            {
                for( int i = 0; i < 1000; i++)
                {
                    cout<<endl;
                }
                cout<<"Unknow error has occured please try again in login()"<<endl;
            }
       
         //getline(cin, space);
    }while( exit_program != "exit");
    
    
}

void Log_in:: locate_account( string input_id, string input_password)
{
    ifstream accounts_file;
    string current_account_id, current_account_password, current_account_pin;
    int current_account_access_level = 0, no_account_found = 0;
    
    accounts_file.open(ACCOUNTS);
    
    if(!accounts_file)
        {
            cout<<"Error opening file"<<endl;
            return;
        }
    //cout<<"file opened successfully"<<endl;
    
    while(!accounts_file.eof())
        {
            accounts_file>>current_account_id;
            accounts_file>>current_account_password;
            accounts_file>>current_account_access_level;
	    accounts_file>>current_account_pin;
    if( current_account_id.compare(input_id)== 0 && current_account_password.compare(input_password) == 0)
        {
         
            accounts_file.close();
            user_password = current_account_password;
            user_id = current_account_id;
            user_access_level = current_account_access_level;
	    user_pin = current_account_pin;

	    try
            {
                    if( user_access_level == 5)
                    {
                        cout<<"user account"<<endl;
                        User_Account client(current_account_id, current_account_password, current_account_pin);
                        
                    }
                    if( user_access_level == 1)
                    {
                        cout<<"admin level account"<<endl;
                        Admin_Account Superuser;
                        
                    }
            }
            catch( int loggin_out_of_account)
            {
                cout<<"******Exiting Account Profile*******"<<endl;
                throw;
            }
            
        }
        else
            {
                throw no_account_found;//return value for try catch.
            }
       
    }
    
 
    
}
void Log_in :: change_alarm_status( string pin)
{
	string input_pin, space;
	
	getline(cin, space);

	cout<<"Enter your pin to alarm the system:"<<endl;
	getline(cin, input_pin);

//	cout<<"User pin is: "<<pin<<endl;
//	cout<<"Input pin is; "<<input_pin<<endl;
	if(pin.compare(input_pin) == 0)
	{
		switch( alarm_flag )
		{
		case 0://activate the alarm
			alarm_flag = 1;
			pthread_t initialize_sensors;
			pthread_create( &initialize_sensors, NULL, &set_up_sensors, NULL);
			pthread_detach( initialize_sensors);
			cout<<"alarm has been activated"<<endl;
			break;
		case 1://deactivate the alarm
			alarm_flag = 0; 
			cout<<"alarm has been deactivated"<<endl;
			break;
		}
	}		
	else
	{ 
		cout<<"incorrect pin"<<endl;
	}

}
/*
 *******************************************************************************************************
                                USER ACCOUNT CLASS BEGINS HERE
 *******************************************************************************************************
 */
User_Account::User_Account(string user_id, string user_password, string user_pin)
{
	account_id = user_id;
	account_password = user_password;
	account_pin = user_pin;
//	cout<<account_pin<<endl;
	display_user_menu();
	
}
void User_Account::display_user_menu()
{
        int user_choice = 0;
    do
    {
        cout<<"Select which function you would like to preform"<<endl;
        cout<<"1: Alarm the System"<<endl;
        cout<<"2: DisAlarm the system"<<endl;
        cout<<"5: To exit the Progarm"<<endl;
        cin>>user_choice;
        cout<<user_choice<<endl;
        
        switch( user_choice )
        {
            case 1://activate the system
		if( alarm_flag ==  0)
		{
		change_alarm_status(account_pin ); 
                }
		else 
			cout<<"alarm is already active"<<endl;
		break;
            case 2://Deactivate the system
		if( alarm_flag == 1)
		{
		change_alarm_status(account_pin);
		}
		else 
			cout<<"alrm is not active"<<endl;
                break;
            case 5://Logs the Client out
                throw user_choice;
                break;
            default:
                cout<<"improper choice input"<<endl;
                break;
        }
    }while( user_choice != 5);
    
        
}

/*
 *********************************************************************************************************
                            Admin_Account CLASS BEGINS HERE
 *********************************************************************************************************
 */

void Admin_Account::display_admin_menu()
{
    int user_choice = 0;
    do
    {
        cout<<"Select which function you would like to preform"<<endl;
        cout<<"1: Rest the a Client Pin"<<endl;
        cout<<"2: Create a new Client"<<endl;
        cout<<"5: To exit the Progarm"<<endl;
        cin>>user_choice;
        
        switch( user_choice )
        {
            case 1://Reset a Clients Pin
                break;
            case 2://Create a new Client Pin
                break;
            case 5://logs the admin out
                throw user_choice;
                break;
            default:
                cout<<"improper choice input please try again: "<<endl;
                break;
        }
    }while( user_choice != 5);
    
    
}
/*
 *********************************************************************************************************
                    thread to constantly check motion and vibration sensory for input
 *********************************************************************************************************
 */
void *set_up_sensors( void *nothing )
    {
        
            //printf("initializing each sensor\n");
            wiringPiSetup();
            pinMode( vibration_pin, INPUT);
            pinMode( motion_pin, INPUT);
            pinMode( speaker_pin, OUTPUT);
            pullUpDnControl( motion_pin, PUD_DOWN);
           // printf("sensors are setup now they will enter into a continous loop");
            
            
            pthread_t sensor_loop;
            pthread_create( &sensor_loop, NULL, &sensors_check_thread, NULL);
            pthread_detach(sensor_loop);
            
            
            
            pthread_exit(NULL);
    }
void *sensors_check_thread( void *nothing_used)
    {
        int vibration_sensor_status = 0;
        int motion_sensor_status = 0;
        
        while(alarm_flag == 1)
        {
	    vibration_sensor_status = digitalRead( vibration_pin );
	    motion_sensor_status = digitalRead( motion_pin );
           // printf("V: %d       M: %d\n", vibration_sensor_status, motion_sensor_status);
            if( vibration_sensor_status == 1 || motion_sensor_status == 1 )
                {
                    digitalWrite( speaker_pin, H);
                  //  printf(" check speaker is going off\n");
                    event_detected = 1;
                    sleep(1);
                }
            else
                {
                   sleep(1);
                }
        }
//	printf("check exited\n");
        digitalWrite( speaker_pin, L);
        pthread_exit( NULL);
        
    }
