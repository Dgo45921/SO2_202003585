
#include <stdbool.h>


bool find_user(int id);


void get_user_information(int account_id);

void do_transaction(int source_account_id, int destination_account_id, float amount);

void do_money_retirement(int account_id, float amount);

void do_deposit(int account_id, float amount);

void do_individual_operation(int operation_type);