#include "helper.h"
 
int main(int argc, char **argv)
{
   // printf("%d", hashFunc("Hello2", 10));

	HashMap map = createHashMap(6);
	insert(&map, "key", "val");
	delete (&map, "key");
	insert(&map, "key", "value");

	char *res = get(&map, "key");
	printf("%s\n", res);

   deleteMap(&map);

   return 0;
}
