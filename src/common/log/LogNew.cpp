/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2008 The EVEmu Team
    For the latest information visit http://evemu.mmoforge.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:     Captnoord
*/

#include "CommonPCH.h"

#include "log/LogNew.h"

#ifndef WIN32
const char* colorstrings[TBLUE+1] = {
    "",
    "\033[22;31m",
    "\033[22;32m",
    "\033[01;33m",
    //"\033[22;37m",
    "\033[0m",
    "\033[01;37m",
    "\033[1;34m",
};
#endif//WIN32

NewLog::NewLog()
{
#ifdef WIN32
    stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif//WIN32

    /* set initial log system time */
    UNIXTIME = time( NULL );

    char * log_filename = (char*)malloc(0x100);
    tm t;
    localtime_r( &UNIXTIME, &t );
    sprintf(log_filename, "log_%02u;%02u;%04u;%02u;%02u.txt", t.tm_mday, t.tm_mon + 1, t.tm_year + 1900, t.tm_hour, t.tm_min);

    m_logfile = fopen(log_filename, "w");
    if ( m_logfile == NULL )
    {
        /* this is because we haven't done shit so we can't do shit..*/
        SetColor( TRED );
        LogTime( stdout );
        printf( "E logging: unable to open: %s | creation of logging system is unsuccessful", log_filename );
        free( log_filename );
        return;
    }

    log("Log system initiated");

    free( log_filename );
}

void NewLog::SetColor(unsigned int color)
{
#ifndef WIN32
    fputs(colorstrings[color], stdout);
#else
    SetConsoleTextAttribute(stdout_handle, (WORD)color);
#endif
}

#if 0
void NewLog::Notice(const char * source, const char * format, ...)
{
#ifdef ENABLE_CONSOLE_LOG
    /* notice is old loglevel 0/string */
    LOCK_LOG;
    va_list ap;
    va_start(ap, format);
    Time();
    fputs("N ", stdout);
    if(*source)
    {
        Color(TWHITE);
        fputs(source, stdout);
        putchar(':');
        putchar(' ');
        Color(TNORMAL);
    }

    vfprintf( m_logfile, format, ap );
    putchar('\n');
    va_end(ap);
    Color(TNORMAL);
    UNLOCK_LOG;
#endif//ENABLE_CONSOLE_LOG
}

#endif


void NewLog::Warning( const char * source, const char * str, ... )
{
#ifdef ENABLE_CONSOLE_LOG
    if(log_level < 2)
        return;

    /* warning is old loglevel 2/detail */
    LOCK_LOG;
    va_list ap;
    va_start(ap, format);
    LogTime( stdout );
    SetColor(TYELLOW);
    fputs("W ", stdout);
    if(*source)
    {
        SetColor(TWHITE);
        fputs(source, stdout);
        putchar(':');
        putchar(' ');
        SetColor(TYELLOW);
    }

    vfprintf( m_logfile, format, ap );
    putchar('\n');
    va_end(ap);
    SetColor(TNORMAL);
    UNLOCK_LOG;
#endif//ENABLE_CONSOLE_LOG
}

void NewLog::Success(const char * source, const char * format, ...)
{
    va_list ap, ap2;
    va_start(ap, format);
    va_copy(ap2, ap); /* this is a design flaw ( UNIX related ) */
    LogTime( stdout ); LogTime( m_logfile );
    SetColor( TGREEN );
    fputs("S ", stdout); fputs("S ", m_logfile);
    if(*source)
    {
        SetColor( TWHITE );
        fputs( source, stdout ); fputs( source, m_logfile );
        fputs( ": ", stdout ); fputs( ": ", m_logfile );
        SetColor( TGREEN );
    }

    vfprintf( stdout, format, ap ); vfprintf( m_logfile, format, ap2 );
    fputc( '\n', stdout ); fputc( '\n', m_logfile );
    va_end(ap);
    va_end(ap2);

    SetColor(TNORMAL);
}

void NewLog::Error( const char * source, const char * format, ... )
{
    va_list ap, ap2;
    va_start(ap, format);
    va_copy(ap2, ap); /* this is a design flaw ( UNIX related ) */
    LogTime( stdout ); LogTime( m_logfile );
    SetColor( TRED );
    fputs("E ", stdout); fputs("E ", m_logfile);
    if( source && *source )
    {
        SetColor( TWHITE );
        fputs( source, stdout ); fputs( source, m_logfile );
        fputs( ": ", stdout ); fputs( ": ", m_logfile );
        SetColor( TRED );
    }

    vfprintf( stdout, format, ap ); vfprintf( m_logfile, format, ap2 );
    fputc( '\n', stdout ); fputc( '\n', m_logfile );
    va_end(ap);
    va_end(ap2);
    SetColor(TNORMAL);
}

void NewLog::SetTime( time_t time )
{
    UNIXTIME = time;
}

NewLog::~NewLog()
{
    log("Log system shutting down");
    if ( m_logfile != NULL)
        fclose( m_logfile );
}
void NewLog::LogTime( FILE* fp )
{
    tm t;
    UNIXTIME = time(NULL); /* this will be replaced my a timing thread somehow */
    localtime_r( &UNIXTIME, &t );
    fprintf( fp, "%02u:%02u:%02u ", t.tm_hour, t.tm_min, t.tm_sec );
}

void NewLog::log( const char * str, ... )
{
    va_list ap;
    va_start( ap, str );
    /* write time */
    LogTime( m_logfile );

    /* write source, which is internal logger */
    fputs( "I Logger:", m_logfile );
    vfprintf( m_logfile, str, ap );
    fputc( '\n', m_logfile );
    va_end( ap );

    /* this is crap but as these messages are high priority I simply don't care */
    fflush( m_logfile );
}

void NewLog::Log( const char * source, const char * str, ... )
{
    va_list ap, ap2;
    va_start(ap, str);
    va_copy(ap2, ap); /* this is a design flaw ( UNIX related ) */
    LogTime( stdout ); LogTime( m_logfile );
    SetColor( TNORMAL );
    fputs("L ", stdout); fputs("L ", m_logfile);
    if( source && *source )
    {
        SetColor( TWHITE );
        fputs( source, stdout ); fputs( source, m_logfile );
        fputs( ": ", stdout ); fputs( ": ", m_logfile );
        SetColor( TNORMAL );
    }

    vfprintf( stdout, str, ap ); vfprintf( m_logfile, str, ap2 );
    fputc( '\n', stdout ); fputc( '\n', m_logfile );
    va_end(ap);
    va_end(ap2);
    SetColor(TNORMAL);

    fflush( m_logfile );
}

void NewLog::Debug(const char * source, const char * format, ...)
{
#ifdef DEBUG
    va_list ap, ap2;
    va_start(ap, format);
    va_copy(ap2, ap); /* this is a design flaw ( UNIX related ) */
    LogTime( stdout ); LogTime( m_logfile );
    SetColor( TBLUE );
    fputs("D ", stdout); fputs("D ", m_logfile);
    if( source && *source )
    {
        SetColor( TWHITE );
        fputs( source, stdout ); fputs( source, m_logfile );
        fputs( ": ", stdout ); fputs( ": ", m_logfile );
        SetColor( TBLUE );
    }

    vfprintf( stdout, format, ap ); vfprintf( m_logfile, format, ap2 );
    fputc( '\n', stdout ); fputc( '\n', m_logfile );
    va_end(ap);
    va_end(ap2);
    SetColor(TNORMAL);
#endif//DEBUG
}