// dma.c

#include "dma.h"

void dma_memcpy(void *dst, const void *src, u16 count)
{
  dma_mem[3].cnt = 0;
  dma_mem[3].src = src;
  dma_mem[3].dst = dst;
  dma_mem[3].cnt = count | DMA_ON;
}
