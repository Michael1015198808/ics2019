#include "common.h"
#include "proc.h"
#include <amdev.h>

static int width=0;
static int height=0;
size_t serial_write(const void *buf, size_t offset, size_t len) {
  VISIT_SLOW();
  int i;
  for(i=0;i<len;++i){
    _putc(*(char*)(buf+i));
  }
  return i;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
    VISIT_SLOW();
    int key=read_key();
    if(key==0){
      return snprintf(buf,len, "t %d\n",uptime());
    }else{
        extern int fg_pcb;
        switch(key){
            case _KEY_F1:
                fg_pcb=1;
                printf("Running pal and hello\n");
                break;
            case _KEY_F2:
                fg_pcb=2;
                printf("Running pal and dummy\n");
                break;
            case _KEY_F3:
                fg_pcb=3;
                printf("Running pal and init\n");
                break;
            case _KEY_F4:
                break;
        }
      return snprintf(buf,len, "k%c %s\n",key&0x8000?'u':'d',keyname[key&0x7fff]);
    }
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  if(len+offset>sizeof(dispinfo)){
    len= sizeof(dispinfo)-offset;
  }
  memcpy(buf, dispinfo + offset, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  VISIT_SLOW();
  int i, x, y;
  offset>>=2;
  y = offset / width;
  x = offset - y * width;
  /*for(i=0;i<len/4;++i){
    int to_print=width-x;
    if(width-x>len/4-i)to_print=len/4-i;
    draw_rect((uint32_t*)buf,x,y,to_print,1);
    i+=width-x;
    x=0;++y;
  }*/
  for(i=0;i<len/4;++i){
    draw_rect((uint32_t*)buf,x,y,len/4-i,1);
    i+=width-x;
    x=0;++y;
  }
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d",width=screen_width(),height=screen_height());

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
