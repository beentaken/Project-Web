#define LABEL_SIZE 16

typedef struct sll_node
{
	char label[LABEL_SIZE];
	int value;
	struct sll_node* next;
}sll_node;

sll_node *sll_add(sll_node *list, int value, const char *label);
sll_node *sll_remove(sll_node *list, int search_value);
sll_node *sll_insert_before(sll_node *list, int search_value, int value, const char *label);
void sll_insert_after(sll_node *list, int search_value, int value, const char *label);
void sll_destroy(sll_node *list);
void sll_dump(const sll_node *list);
