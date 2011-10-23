/* -*- C -*-
 * File: libraw_datastream.h
 * Copyright 2008-2009 LibRaw LLC (info@libraw.org)
 * Created: Sun Jan 18 13:07:35 2009
 *
 * LibRaw (Lite) Data stream interface
 *
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __LIBRAW_DATASTREAM_H
#define __LIBRAW_DATASTREAM_H

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#ifndef __cplusplus

struct LibRaw_abstract_datastream;

#else /* __cplusplus */

#include "libraw_const.h"

class LibRaw_buffer_datastream;

class LibRaw_abstract_datastream
{
  public:
    LibRaw_abstract_datastream(){substream=0;};
    virtual             ~LibRaw_abstract_datastream(void){if(substream) delete substream;}
    virtual int         valid(){return 0;}
    virtual int         read(void *,size_t, size_t ){ return -1;}
    virtual int         seek(off_t o, int whence){return -1;}
    virtual int         tell(){return -1;}
    virtual int         get_char(){return -1;}
    virtual char*       gets(char *, int){ return NULL;}
    virtual int         scanf_one(const char *, void *){return -1;}
    virtual int         eof(){return -1;}

    virtual const char* fname(){ return NULL;};
    virtual int         subfile_open(const char*){ return EINVAL;}
    virtual void        subfile_close(){}
    virtual int		tempbuffer_open(void*, size_t);
    virtual void	tempbuffer_close()
    {
        if(substream) delete substream;
        substream = NULL;
    }

  protected:
    LibRaw_abstract_datastream *substream;
};


class LibRaw_file_datastream : public LibRaw_abstract_datastream
{
  public:
    LibRaw_file_datastream(const char *fname) 
        { 
            if(fname)
                {filename = fname; f = fopen(fname,"rb");}
            else 
                {filename=0;f=0;}
            sav=0;
        }

    virtual ~LibRaw_file_datastream() {if(f)fclose(f); if(sav)fclose(sav);}

    virtual int valid() { return f?1:0;}

#define CHK() do {if(!f) throw LIBRAW_EXCEPTION_IO_EOF;}while(0)
    virtual int read(void * ptr,size_t size, size_t nmemb) 
    { 
        CHK(); 
        return substream?substream->read(ptr,size,nmemb):int(fread(ptr,size,nmemb,f));
    }
    virtual int eof() 
    { 
        CHK(); 
        return substream?substream->eof():feof(f);
    }
    virtual int seek(off_t o, int whence) 
    { 
        CHK(); 
        return substream?substream->seek(o,whence):fseek(f,o,whence);
    }
    virtual int tell() 
    { 
        CHK(); 
        return substream?substream->tell():ftell(f);
    }
    virtual int get_char() 
    { 
        CHK(); 
        return substream?substream->get_char():fgetc(f);
    }
    virtual char* gets(char *str, int sz) 
    { 
        CHK(); 
        return substream?substream->gets(str,sz):fgets(str,sz,f);
    }
    virtual int scanf_one(const char *fmt, void*val) 
    { 
        CHK(); 
        return substream?substream->scanf_one(fmt,val):fscanf(f,fmt,val);
    }

    virtual const char *fname() { return filename; }

    virtual int subfile_open(const char *fn)
    {
        if(sav) return EBUSY;
        sav = f;
        f = fopen(fn,"rb");
        if(!f)
            {
                f = sav;
                sav = NULL;
                return ENOENT;
            }
        else
            return 0;
    }
    virtual void subfile_close()
    {
        if(!sav) return;
        fclose(f);
        f = sav;
        sav = 0;
    }

  private:
    FILE *f,*sav;
    const char *filename;
};
#undef CHK

class LibRaw_buffer_datastream : public LibRaw_abstract_datastream
{
  public:
    LibRaw_buffer_datastream(void *buffer, size_t bsize)
        {
            buf = (unsigned char*)buffer; streampos = 0; streamsize = bsize;
        }
    virtual ~LibRaw_buffer_datastream(){}
    virtual int valid() { return buf?1:0;}
    virtual int read(void * ptr,size_t sz, size_t nmemb) 
    { 
        if(substream) return substream->read(ptr,sz,nmemb);
        size_t to_read = sz*nmemb;
        if(to_read > streamsize - streampos)
            to_read = streamsize-streampos;
        if(to_read<1) 
            return 0;
        memmove(ptr,buf+streampos,to_read);
        streampos+=to_read;
        return int((to_read+sz-1)/sz);
    }

    virtual int eof() 
    { 
        if(substream) return substream->eof();
        return streampos >= streamsize;
    }

    virtual int seek(off_t o, int whence) 
    { 
        if(substream) return substream->seek(o,whence);
        switch(whence)
            {
            case SEEK_SET:
                if(o<0)
                    streampos = 0;
                else if (size_t(o) > streamsize)
                    streampos = streamsize;
                else
                    streampos = size_t(o);
                return 0;
            case SEEK_CUR:
                if(o<0)
                    {
                        if(size_t(-o) >= streampos)
                            streampos = 0;
                        else
                            streampos += o;
                    }
                else if (o>0)
                    {
                        if(o+streampos> streamsize)
                            streampos = streamsize;
                        else
                            streampos += o;
                    }
                return 0;
            case SEEK_END:
                if(o>0)
                    streampos = streamsize;
                else if ( size_t(-o) > streamsize)
                    streampos = 0;
                else
                    streampos = streamsize+o;
                return 0;
            default:
                return 0;
            }
    }
    
    virtual int tell() 
    { 
        if(substream) return substream->tell();
        return int(streampos);
    }

    virtual int get_char() 
    { 
        if(substream) return substream->get_char();
        if(streampos>=streamsize)
            return -1;
        return buf[streampos++];
    }
    virtual char* gets(char *s, int sz) 
    { 
        if (substream) return substream->gets(s,sz);
        unsigned char *psrc,*pdest,*str;
        str = (unsigned char *)s;
        psrc = buf+streampos;
        pdest = str;
        while ( (size_t(psrc - buf) < streamsize)
               &&
                ((pdest-str)<sz)
		)
	  {
                *pdest = *psrc;
                if(*psrc == '\n')
                    break;
                psrc++;
                pdest++;
            }
        if(size_t(psrc-buf) < streamsize)
            psrc++;
        if((pdest-str)<sz)
            *(++pdest)=0;
        streampos = psrc - buf;
        return s;
    }
    virtual int scanf_one(const char *fmt, void* val) 
    { 
        if(substream) return substream->scanf_one(fmt,val);
        int scanf_res;
        if(streampos>streamsize) return 0;
        scanf_res = sscanf((char*)(buf+streampos),fmt,val);
        if(scanf_res>0)
            {
                int xcnt=0;
                while(streampos<streamsize)
                    {
                        streampos++;
                        xcnt++;
                        if(buf[streampos] == 0
                           || buf[streampos]==' '
                           || buf[streampos]=='\t'
                           || buf[streampos]=='\n'
                           || xcnt>24)
                            break;
                    }
            }
        return scanf_res;
    }
  private:
    unsigned char *buf;
    size_t   streampos,streamsize;
};

inline int LibRaw_abstract_datastream::tempbuffer_open(void  *buf, size_t size)
{
    if(substream) return EBUSY;
    substream = new LibRaw_buffer_datastream(buf,size);
    return substream?0:EINVAL;
}


#endif

#endif

