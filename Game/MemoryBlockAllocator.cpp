//
//  MemoryBlockAllocator.cpp
//  RTSGame
//
//  Created by Arshad on 17/05/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#include "MemoryBlockAllocator.h"
#include <string>
#include <cassert>

namespace memory
{

// Memory Block size for each Heap
int memoryBlockSize[] =
{
    8,
    16,
    32,
    48,
    64,
    96,
    128,
    144,
    160,
    176,
    192,
    224,
    256,
    288,
    320,
    352,
    384,
    416,
    480,
    512,
    544,
    576,
    608,
    640,
    672,
    704,
    736,
    768,
    800,
    832,
    864,
    896,
    928,
    960,
    992,
    1024
};

int MemoryBlockAllocator::m_HeapLookUpTable[MAX_OBJECT_SIZE+1];

MemoryBlockAllocator::MemoryBlockAllocator()
{
    m_HeapCount = NUM_HEAPS;
    m_pHeapArray = (Heap*)malloc(m_HeapCount * sizeof(Heap));
    m_CurrentHeapCount = 0;
    
    memset(m_pFreeMemoryBlock, 0, NUM_HEAPS * sizeof(MemoryBlock));
    // initialize the heap lookup table.
    static bool bOnce = true;
    if( bOnce )
    {
        bOnce = false;
        int n = 0;
        for (int i=0; i<=MAX_OBJECT_SIZE; i++)
        {
            if( i  <= memoryBlockSize[n] )
            {
                m_HeapLookUpTable[i] = n;
            }
            else
            {
                n++;
                m_HeapLookUpTable[i] = n;
            }
        }
    }
}

void* MemoryBlockAllocator::Allocate(int size)
{
    if( size == 0 )
        return 0;
    
    if( size > MAX_OBJECT_SIZE )
    {
        return malloc(size);
    }
    // Find out the Heap from which Memory Block will be returned.
    int heapIndex = m_HeapLookUpTable[size];
    
    // We have Free Memory Block available in this Heap
    if( m_pFreeMemoryBlock[heapIndex] )
    {
        MemoryBlock* pBlock = m_pFreeMemoryBlock[heapIndex];
        // set the Free pointer to the Next Memory Block
        m_pFreeMemoryBlock[heapIndex] = pBlock->m_pNext;
        return pBlock;
    }
    // We don't have Free memory block in this Heap, so we need to create a new Heap space
    else
    {
        // All the Heaps are Full. need to create new ones.
        if( m_CurrentHeapCount == m_HeapCount )
        {
            assert(1);
            m_HeapCount *= 2;
            Heap* newHeap = (Heap*)malloc(m_HeapCount * sizeof(Heap));
            memcpy(newHeap, m_pHeapArray, sizeof(Heap) * m_CurrentHeapCount);
            free(m_pHeapArray);
            m_pHeapArray = newHeap;
        }
        Heap* pHeap = m_pHeapArray + m_CurrentHeapCount;
        int blockSize = memoryBlockSize[heapIndex];
        
        pHeap->m_pMemoryChunk = (MemoryBlock*)malloc( MEMORY_CHUNK_SIZE );
        pHeap->m_MemoryBlockSize = blockSize;
        int blockCount = MEMORY_CHUNK_SIZE/blockSize;
        
        MemoryBlock* pBlock;
        for (int i=0; i<blockCount-1; i++)
        {
            pBlock = (MemoryBlock*)( (char*)pHeap->m_pMemoryChunk +  blockSize * i );
            pBlock->m_pNext = (MemoryBlock*)( (char*)pHeap->m_pMemoryChunk + blockSize * (i+1));
        }
        pBlock = (MemoryBlock*)( (char*)pHeap->m_pMemoryChunk +  blockSize * (blockCount-1) );
        pBlock->m_pNext = 0;
        
        m_pFreeMemoryBlock[heapIndex] = pHeap->m_pMemoryChunk->m_pNext;
        
        m_CurrentHeapCount++;
        //080-67195113
        return pHeap->m_pMemoryChunk;
    }
    
    assert(0);
    return 0;
}

void MemoryBlockAllocator::Free(void* ptr, int size)
{
    if( size == 0 )
        return;
    if( size > 1024 )
    {
        free(ptr);
        return;
    }
    // Find out the Heap from which Memory Block will be returned.
    int heapIndex = m_HeapLookUpTable[size];
    
    MemoryBlock* pBlock = m_pFreeMemoryBlock[heapIndex];
    m_pFreeMemoryBlock[heapIndex] = (MemoryBlock*)ptr;
    ((MemoryBlock*)ptr)->m_pNext = pBlock;
}

MemoryBlockAllocator::~MemoryBlockAllocator()
{
    for( int i=0; i<m_HeapCount; i++ )
    {
        Heap* pHeap = m_pHeapArray + i;
        free(pHeap->m_pMemoryChunk);
    }
    free(m_pHeapArray);
}
    
}
