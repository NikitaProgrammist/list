#ifndef TIME_MEMORY_H
#define TIME_MEMORY_H

#include <boost/pool/pool_alloc.hpp>
#include <boost/container/list.hpp>

void check();
void stdInsert(std::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> * l1);
void boostInsert(boost::container::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> * l3);
void myInsert(List * l2);
void stdDelete(std::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> * l1);
void boostDelete(boost::container::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> * l3);
void myDelete(List * l2);

#endif // TIME_MEMORY_H
