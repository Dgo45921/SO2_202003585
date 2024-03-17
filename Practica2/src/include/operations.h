
#include <stdbool.h>


int find_user(int id);


void get_user_information(int account_id);

int do_transaction(int source_account_id, int destination_account_id, float amount, bool register_error);

int do_money_retirement(int account_id, float amount, bool register_error);

int do_deposit(int account_id, float amount, bool register_error);

void do_individual_operation(int operation_type);