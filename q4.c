#include <stdio.h>
#include <string.h>

struct Book
{
    int id;
    int popularity;
    int lastUsed;
};

int main()
{
    int capacity, Q;
    int i, j;

    scanf("%d %d", &capacity, &Q);

    struct Book shelf[capacity];

    for (i = 0; i < capacity; i++)
        shelf[i].id = -1;

    int time = 0;

    while (Q--)
    {
        char op[10];
        int x, y;

        scanf("%s", op);

        if (strcmp(op, "ADD") == 0)
        {
            scanf("%d %d", &x, &y);

            int found = -1;
            for (i = 0; i < capacity; i++)
                if (shelf[i].id == x)
                    found = i;

            if (found != -1)
            {
                shelf[found].popularity = y;
                shelf[found].lastUsed = time++;
            }
            else
            {
                int empty = -1;
                for (i = 0; i < capacity; i++)
                    if (shelf[i].id == -1)
                        empty = i;

                if (empty != -1)
                {
                    shelf[empty].id = x;
                    shelf[empty].popularity = y;
                    shelf[empty].lastUsed = time++;
                }
                else
                {
                    int lru = 0;
                    for (i = 1; i < capacity; i++)
                        if (shelf[i].lastUsed < shelf[lru].lastUsed)
                            lru = i;

                    shelf[lru].id = x;
                    shelf[lru].popularity = y;
                    shelf[lru].lastUsed = time++;
                }
            }
        }

        else if (strcmp(op, "ACCESS") == 0)
        {
            scanf("%d", &x);

            int found = -1;
            for (i = 0; i < capacity; i++)
                if (shelf[i].id == x)
                    found = i;

            if (found == -1)
                printf("-1\n");
            else
            {
                printf("%d\n", shelf[found].popularity);
                shelf[found].lastUsed = time++;
            }
        }
    }

    return 0;
}