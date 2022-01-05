#include "operations.h"

#include <memory.h>

bool TryGetVirtualMemoryPointer(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                VirtualMemoryBaseType address,
                                VirtualMemoryPointer *pointer)
{
   if(address < handle->Constant->AddressingStart || address > handle->Dynamic.AddressingEnd)
      return false;

   VirtualMemoryBaseType chunkLastAddress = -1;
   address -= handle->Constant->AddressingStart;

   for(VirtualMemoryBaseType i = 0; i < handle->Constant->ChunksList->Count; i++)
   {
      const VirtualMemoryChunk *chunk = &handle->Constant->ChunksList->Chunks[i];
      size_t chunkBytes = chunk->BytesCount;

      chunkLastAddress += chunkBytes;

      if(address <= chunkLastAddress)
      {
         *pointer = (VirtualMemoryPointer)
         {
            .Chunk = chunk,
            .Offset = address - (chunkLastAddress - chunkBytes + 1)
         };

         return true;
      }
   }

   return false;
}

VirtualMemoryStatus ReadVirtualMemoryChunk(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                           const VirtualMemoryChunk *chunk,
                                           const VirtualMemoryChunkParameters *parameters,
                                           const void *callContext)
{
   if(chunk->Read != NULL)
      return chunk->Read(handle, &parameters->AsRead, chunk->Context, callContext);

   memset(parameters->AsRead.Data, __VIRTUAL_MEMORY_MOCK_VALUE, parameters->AsRead.Size);

   return VIRTUAL_MEMORY_STATUS_OK;
}

VirtualMemoryStatus WriteVirtualMemoryChunk(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                            const VirtualMemoryChunk *chunk,
                                            const VirtualMemoryChunkParameters *parameters,
                                            const void *callContext)
{
   if(chunk->Write != NULL)
      return chunk->Write(handle, &parameters->AsWrite, chunk->Context, callContext);

   return VIRTUAL_MEMORY_STATUS_OK;
}
