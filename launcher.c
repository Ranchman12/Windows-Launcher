/*
  Course: CS 30200
  Name: Alex Lowther
  Assignment: 2
  
*/



    #include <windows.h> 
    #include <stdio.h>
    # include <stdlib.h>
    
    void printError(char* functionName);
    
    int main(void){
       int input=1;
       char lpCommandLine[100];
       PROCESS_INFORMATION processInfo;
       STARTUPINFO startInfo;
       ZeroMemory(&startInfo, sizeof(startInfo));
       startInfo.cb = sizeof(startInfo);
      
      while(input != 0 ){
   
           printf("%s", "Please make a choice from the following list. \n");
           printf("%s", " 0: Quit \n");
           printf("%s", " 1: Run Notepad \n");
           printf("%s", "*2: Run cmd shell \n");
           printf("%s", "#3: Run NS LooKUp \n");
           printf("%s", " 4: Run Character Map \n");
           printf("%s", " 5: Run WordPad \n");  
           printf("Enter your choice now:");
           //scan user input(and display menu) after while condition
           //to by pass other system directories we can use systemroot
           scanf("%d", &input);
           sprintf(lpCommandLine, getenv("SYSTEMROOT"));
           //case by case, we concatinate the file name to the user's root 
          switch(input){
               case 0:
                   printf("ended");
                   return 0;
                   break;
               case 1: 
                   sprintf(lpCommandLine, "%s\\notepad.exe", lpCommandLine);
                   break;
               case 2:
               //STARTFUSPOSITION put window in left corner
               //mis red and green makes yellow
                   sprintf(lpCommandLine, "%s\\system32\\cmd.exe", lpCommandLine);
                   startInfo.dwFlags = STARTF_USEFILLATTRIBUTE |  STARTF_USEPOSITION;
                   startInfo.dwFillAttribute =  FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED;      
                   startInfo.lpTitle = "What is your command?";
                  _putenv("PROMPT=Speak up me$g");
               break;
               case 3: sprintf(lpCommandLine, "%s\\system32\\nslookup.exe", lpCommandLine);
               break;
               case 4:
                   sprintf(lpCommandLine, "%s\\system32\\charmap.exe", lpCommandLine);
                   break;
               case 5:
                   sprintf(lpCommandLine, "%s\\write.exe", lpCommandLine);
                   break;
               default:
                   printf("please enter numbers 0 to 5 only");
            }
           //if user chooses nslookup, we must exit or else we get never ending loop
           //creation flags in createprocess have to be changed CREATE_SHARED_WOW_VDM creates the process in the same window
           //inheritied handle is true
           
                   if(input == 3){
                         printf("\n");
                                  if(!CreateProcess(NULL,
                                                    lpCommandLine,
                                                    NULL,         
                                                    NULL,         
                                                    TRUE,         
                                                    NORMAL_PRIORITY_CLASS | CREATE_SHARED_WOW_VDM,           
                                                    NULL,         
                                                    NULL,         
                                                    &startInfo, 
                                                    &processInfo)){printError("CreateProcess");}
                                                    //display pid using dwprocessid          
                                                    printf("Started program %d with PID = %d\n\n", input, (int)processInfo.dwProcessId);
                                                    //this condition will be true, we can grab the exit code using a DWORD object
                                                    if(WaitForSingleObject(processInfo.hProcess, INFINITE) == WAIT_OBJECT_0){
                                                      DWORD exitValue;
                                                      GetExitCodeProcess(processInfo.hProcess, &exitValue);
                                                      printf(" Program %d exited with return value %d\n", input, (int)exitValue);
                                                     }
                                                   
                                                   CloseHandle(processInfo.hThread);
                                                   CloseHandle(processInfo.hProcess);   
                                 }      
                               
                   //for all other choices, keep flags CREATE_NEW_CONSOLE
                   if(input != 3){
                                    if(!CreateProcess(NULL,
                                                        lpCommandLine,
                                                        NULL,
                                                        NULL,
                                                        FALSE,
                                                        NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
                                                        NULL,
                                                        NULL,
                                                        &startInfo,
                                                        &processInfo)) {printError("CreateProcess");}   
                                                        
                        printf("Started program %d with PID = %d\n\n", input, (int)processInfo.dwProcessId);
                        printf("\n");
                        //if user chooses cmd, we have to wait for exit value just like nslookup
                        //this condition must be placed here becuase we create a new console like other choices
                           if(input == 2){
                                              printf("  waiting for program %d to terminate.\n", input);
                                              if(WaitForSingleObject(processInfo.hProcess, INFINITE) == WAIT_OBJECT_0){
                                                 DWORD exitValue;
                                                 GetExitCodeProcess(processInfo.hProcess, &exitValue);
                                                 printf(" Program %d exited with return value %d\n", input, (int)exitValue);
                                              }
                                           CloseHandle(processInfo.hThread);
                                           CloseHandle(processInfo.hProcess);
                                          }
                                }
                        }
            }
            
    
    
    /****************************************************************
       The following function can be used to print out "meaningful"
       error messages. If you call a Win32 function and it returns
       with an error condition, then call this function right away and
       pass it a string containing the name of the Win32 function that
       failed. This function will print out a reasonable text message
       explaining the error and then (if chosen) terminate the program.
    */
    void printError(char* functionName)
    {
       LPVOID lpMsgBuf;
       int error_no;
       error_no = GetLastError();
       FormatMessage(
             FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
             NULL,
             error_no,
             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
             (LPTSTR) &lpMsgBuf,
             0,
             NULL
       );
       // Display the string.
       fprintf(stderr, "\n%s failed on error %d: ", functionName, error_no);
       fprintf(stderr, (char*)lpMsgBuf);
       // Free the buffer.
       LocalFree( lpMsgBuf );
       //ExitProcess(1);  // terminate the program
    } //printError
