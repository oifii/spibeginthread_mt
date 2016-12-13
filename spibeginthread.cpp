/*
 * Copyright (c) 2010-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Windows.h>
//#include <process.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <WinBase.h>

using namespace std;




//    User Defines Messages
#define        THRD_MESSAGE_SOMEWORK        WM_USER + 1
#define        THRD_MESSAGE_EXIT            WM_USER + 2
//    Application Specific Preprocessor definitions
#define        NUM_THREADS                    10

DWORD WINAPI    ThrdFunc ( LPVOID n )
{
    int    TNumber    =    ( int )n;
    //    Here we will wait for the messages
    while ( 1 )
    {
        MSG    msg;
        //BOOL    MsgReturn  =  PeekMessage ( &msg , NULL , 
        //    THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT , PM_REMOVE );
        BOOL    MsgReturn    =    GetMessage ( &msg , NULL , 
            THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT );
            
        if ( MsgReturn )
        {
            switch ( msg.message )
            {
            case THRD_MESSAGE_SOMEWORK:
                cout<<"Working Message.... for Thread Number "
                   <<TNumber<<endl;
                break;
            case THRD_MESSAGE_EXIT:
                cout<<"Exiting Message... for Thread Number "
                   <<TNumber<<endl;
                return 0;
            }
        }
    }
    
    return 0;
}

int main()
{
    HANDLE    hThrd [ NUM_THREADS ];
    DWORD    Id [ NUM_THREADS ];
    short LoopCounter = 0;
    //    Create all the threads
    for ( ; LoopCounter < NUM_THREADS ; LoopCounter ++ )
    {
        hThrd [ LoopCounter ] = CreateThread ( NULL , 0 , 
            (LPTHREAD_START_ROUTINE)ThrdFunc , 
            (LPVOID)LoopCounter , 0, &Id [ LoopCounter ] );
        if ( !hThrd )
        {
            cout<<"Error Creating Threads,,,,.exiting"<<endl;
            return -1;
        }
        Sleep ( 100 );
    }
    Sleep ( 1000 );
    //    Send Working Message to all Threads 
    for ( LoopCounter = 0; LoopCounter < NUM_THREADS ; LoopCounter ++ )
    {
        PostThreadMessage ( Id [ LoopCounter ] , 
            THRD_MESSAGE_SOMEWORK , 0 , 0 );
        Sleep ( 100 );
    }
    //    Sleep againg for 1 seconds and send exit message to all threads
    Sleep ( 10000 );
    for ( LoopCounter = 0; LoopCounter < NUM_THREADS ; LoopCounter ++ )
    {
        PostThreadMessage ( Id [ LoopCounter ] , THRD_MESSAGE_EXIT, 0 , 0 );
        Sleep ( 100 );
    }
    //    Wait for all threads to exit
    WaitForMultipleObjects ( NUM_THREADS, hThrd , true , INFINITE );
    //    Close All handles
    for ( LoopCounter = 0; LoopCounter < NUM_THREADS ; LoopCounter ++ )
        //CloseHandler ( hThrd [ LoopCounter ] );
        CloseHandle ( hThrd [ LoopCounter ] );

    return 0;
}