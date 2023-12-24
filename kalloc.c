// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
} kmem;

//free pages 선언 
uint num_free_pages;

//페이지들에 대한 reference counter 선언 
uint pgrefcount[PHYSTOP >> PTXSHIFT];

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just

// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{ 
  initlock(&kmem.lock, "kmem");
  //0으로 초기화 
  num_free_pages = 0;
  kmem.use_lock = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE){
    pgrefcount[V2P(p) >> PTXSHIFT] = 0;
    kfree(p);
  }
}

//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  if(kmem.use_lock)
    acquire(&kmem.lock);
  
  
  //해당 페이지를 참고하는 경우가 존재하므로, refcount 값만 감소 
  if(get_refcount(V2P(v)) > 0)
    dec_refcount(V2P(v));

  //더 이상 참조하는 경우가 없으므로, free_pages값 증가 
  if(get_refcount(V2P(v)) == 0){
    // Fill with junk to catch dangling refs.
    memset(v, 1, PGSIZE);
    //freepages 하나 증가 
    num_free_pages++;
    r = (struct run*)v;
    r->next = kmem.freelist;
    kmem.freelist = r;
  }  
  
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;
  
  if(kmem.use_lock)
    acquire(&kmem.lock);
  //free_pages 하나 감소 
  num_free_pages--;
  
  r = kmem.freelist;
  if(r){
    kmem.freelist = r->next;
    //page 할당 되었으므로, refcount의 값은 1로 초기화 
    pgrefcount[V2P((char*)r) >> PTXSHIFT] = 1;
  }
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}



uint 
getNumFreePages(void)
{
  return num_free_pages;
}

uint get_refcount(uint pa){
  return pgrefcount[pa >> PTXSHIFT];
}

void inc_refcount(uint pa){
  ++pgrefcount[pa >> PTXSHIFT];
}

void dec_refcount(uint pa){
    --pgrefcount[pa >> PTXSHIFT];
}
