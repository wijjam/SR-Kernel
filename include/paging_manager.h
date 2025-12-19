#ifndef PAGING_MANAGER_H
#define PAGING_MANAGER_H



void setup_entries();

uint32_t get_physical_address(uint32_t viritual_adress);

void set_CR3_register();

void set_CR0_32_bit_register(int on);

void map_virtual_to_physical_address(uint32_t virtual_address, uint32_t choosen_physical_address);

void init_paging();



#endif