#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include "STFQNWScheduler.h"
#include "util.h"

STFQNWScheduler::STFQNWScheduler(long bytesPerSec)
{
  maxRate = bytesPerSec;
  virtualTime = 0;
  maxFinishTag = 0;
  nextDeadline = 0;
  waitingBuffers = 0;

  smutex_init(&lock);
  scond_init(&okToSend);
}

bool operator<(const buffer &a, const buffer &b)
{
  return a.startTag > b.startTag;
}

void 
STFQNWScheduler::waitMyTurn(int flowId, float weight, int lenToSend)
{
  smutex_lock(&lock);

  buffer buff;
  buff.startTag = std::max(prevFinishTag[flowId], virtualTime);
  buff.finishTag = buff.startTag + lenToSend/weight;
  maxFinishTag = std::max(buff.finishTag,maxFinishTag);
  pQueue.push(buff);
  if(pQueue.empty() && waitingBuffers == 0)
  {
	virtualTime = maxFinishTag;
  }
  while(nowMS() < nextDeadline || buff.startTag != pQueue.top().startTag)
  {
	waitingBuffers++;
	scond_wait(&okToSend, &lock);
	waitingBuffers--;
  }
  virtualTime = buff.startTag;
  nextDeadline = nowMS() + (1000*lenToSend/maxRate);
  prevFinishTag[flowId] = buff.finishTag;
  pQueue.pop();

  smutex_unlock(&lock);
}

long long 
STFQNWScheduler::signalNextDeadline(long long prevDeadline)
{
  smutex_lock(&lock);

  scond_signal(&okToSend,&lock);

  smutex_unlock(&lock);

  return nextDeadline;
}


