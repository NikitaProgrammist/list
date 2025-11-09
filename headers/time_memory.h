#ifndef TIME_MEMORY_H
#define TIME_MEMORY_H

#include <boost/pool/pool_alloc.hpp>
#include <boost/container/list.hpp>

void check();
void stdInsert(std::list<double, boost::fast_pool_allocator<double>> * l1);
void boostInsert(boost::container::list<double, boost::fast_pool_allocator<double>> * l3);
void myInsert(List * l2);
void stdDelete(std::list<double, boost::fast_pool_allocator<double>> * l1);
void boostDelete(boost::container::list<double, boost::fast_pool_allocator<double>> * l3);
void myDelete(List * l2);

#endif // TIME_MEMORY_H
