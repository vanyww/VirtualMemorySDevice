#include "chunk_finder.h"

bool VirtualMemoryTryFindChunk(__SDEVICE_HANDLE(VirtualMemory) *handle,
                               VirtualMemorySDeviceBaseType address,
                               VirtualMemoryPointer *pointer)
{
   if(address < handle->Constant->AddressingStart)
      return false;

   VirtualMemorySDeviceBaseType chunkLastAddress = -1;
   address -= handle->Constant->AddressingStart;

   for(VirtualMemorySDeviceBaseType i = 0; i < handle->Constant->ChunksCount; i++)
   {
      chunkLastAddress += handle->Constant->Chunks[i].BytesCount;

      if(address <= chunkLastAddress)
      {
         *pointer = (VirtualMemoryPointer)
         {
            .Chunk = &handle->Constant->Chunks[i],
            .Offset = address - (chunkLastAddress - handle->Constant->Chunks[i].BytesCount + 1)
         };

         return true;
      }
   }

   return false;
}
