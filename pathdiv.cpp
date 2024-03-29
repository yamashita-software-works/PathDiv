/*
 *
 *  PathDiv.cpp
 *
 *  Create 2021.05.26
 *
 *  Author YAMASHITA Katsuhiro
 *
 */
#include <ntifs.h>
#include <stdio.h>
#include <malloc.h>
#include <strsafe.h>
#include <locale.h>
#include "ntnativeapi.h"

BOOLEAN bHeader = FALSE;

//----------------------------------------------------------------------------
//
//  wmain()
//
//----------------------------------------------------------------------------
int __cdecl wmain(int argc, WCHAR* argv[])
{
    NTSTATUS Status;
    UNICODE_STRING name={0};
    UNICODE_STRING val={0};
    WCHAR envValBuffer[32768];
    WCHAR *envName = NULL;
    int i;

    setlocale(LC_ALL,"");

    for(i = 1; i < argc; i++)
    {
        if( argv[i][0] == L'-' || argv[i][0] == L'/' )
        {
            if( argv[i][1] == L'e' || argv[i][1] == L'E' )
            {
                bHeader = TRUE;
            }
        }
        else
        {
            if( envName == NULL )
            {
                envName = argv[i]; // name only available which found first.
            }
        }
    }

    if( envName == NULL )
    {
        envName = L"PATH";
    }

    // Block: Read environment variable
    {
        RtlInitUnicodeString(&name,envName);
        RtlZeroMemory(envValBuffer,sizeof(envValBuffer));
        RtlInitEmptyUnicodeString(&val,envValBuffer,32767);

        Status = RtlQueryEnvironmentVariable_U(NULL,&name,&val);
        if( Status != STATUS_SUCCESS )
        {
            if( Status == STATUS_VARIABLE_NOT_FOUND )
                printf("Environment variable not found.\n");
            else
                printf("Environment variable error - 0x%08X\n",Status);
            return 0;
        }
    }

    WCHAR  cf;
    PCWSTR seps = L";";
    PWSTR  token = wcstok( val.Buffer,seps );
    while( token != NULL )
    {
        if( bHeader )
        {
            cf = RtlDoesFileExists_U(token) ? L' ' : L'*';
            wprintf(L"%c %s\n",cf,token);
        }
        else
        {
            wprintf(L"%s\n",token);
        }

        token = wcstok( NULL, seps );
    }

    return 0;
}
