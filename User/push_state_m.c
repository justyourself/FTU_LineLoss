#include "TypeRAM.h"
#include "data.h"

void Insert_Push(unsigned char type,unsigned char ch)
{
  int i;
  for(i=0;i<MAX_CH_NUM;++i)
  {
    if(Push_state[i].valid==0)
    {
      Push_state[i].Type=type;
      Push_state[i].Ch=ch;
      Push_state[i].valid=1;
      break;
    }
  }
}

int Get_Valid_Push(PUSHSTATE *buf)
{
  int i;
  if(buf==0)
    return 0;
  for(i=0;i<MAX_CH_NUM;++i)
  {
    if(Push_state[i].valid)
    {
      buf->Type = Push_state[i].Type;
      buf->Ch = Push_state[i].Ch;
      buf->valid = Push_state[i].valid;
      Push_state[i].valid = 0;
      return 1;
    }
  }
  return 0;
}