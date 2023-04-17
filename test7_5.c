#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAZE_SIZE 10
#define MAX_STACK_SIZE 101



typedef struct {
    short r;
    short c;
} element;

typedef struct {
    int top;
    element data[MAX_STACK_SIZE];
} StackType;

void init_stack(StackType* s) {
    s->top = -1;
}

int is_empty(StackType* s) {
    return (s->top == -1);
}

int is_full(StackType* s) {
    return (s->top == MAX_STACK_SIZE - 1);
}

void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "Stack Full");
        return;
    }
    else {
        s->data[++(s->top)] = item;
    }
}

element pop(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "Stack Empty");
        element err = { -1, -1 };
        return err;
    }
    else {
        return (s->data[(s->top)--]);
    }
}

element peek(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "StackEmpty");
        element err = { -1,-1 };
        return err;
    }
    else {
        return(s->data[s->top]);
    }
}

element here = { 1, 0 }, entry = { 1, 0 };

char maze[MAZE_SIZE][MAZE_SIZE] = {
        {'1','1','1','1','1','1','1','1','1','1'},
        {'e','1','0','1','0','0','0','1','0','1'},
        {'0','0','0','1','0','0','0','1','0','1'},
        {'0','1','0','0','0','1','1','0','0','1'},
        {'1','0','0','0','1','0','0','0','0','1'},
        {'1','0','0','0','1','0','0','0','0','1'},
        {'1','0','0','0','0','0','1','0','1','1'},
        {'1','0','1','1','1','0','1','1','0','1'},
        {'1','1','0','0','0','0','0','0','0','x'},
        {'1','1','1','1','1','1','1','1','1','1'}
};

char visited[MAZE_SIZE][MAZE_SIZE] = { 0 };

void push_loc(StackType* s, int r, int c) {
    if (r < 0 || c < 0) {
        return;
    }
    if (maze[r][c] != '1' && maze[r][c] != '.' && !visited[r][c]) {
        element tmp;
        tmp.r = r;
        tmp.c = c;
        push(s, tmp);
    }
}

void maze_print(char maze[MAZE_SIZE][MAZE_SIZE]) {
    printf("\n");
    for (int r = 0; r < MAZE_SIZE; r++) {
        for (int c = 0; c < MAZE_SIZE; c++) {
            printf("%c", maze[r][c]);
        }
        printf("\n");
    }
}

void print_path(StackType* s) {
    printf("Path: \n");

    element prev = { -1, -1 };
    int step = 0;
    for (int i = 0; i <= s->top; i++) {
        element current = s->data[i];
        if (current.r == prev.r && current.c == prev.c) {
            continue;
        }
        printf("Step %d: (%d, %d)\n", step, current.r, current.c);
        step++;
        prev = current;
    }
    printf("\n");
}


int main(void) {
    int r, c;
    StackType s1; 
    StackType s2;
    StackType s3;

    init_stack(&s1);
    init_stack(&s2);
    init_stack(&s3);
    here = entry;
    push(&s2, here); 
    push(&s3, here);
    while (maze[here.r][here.c] != 'x') {
        r = here.r;
        c = here.c;
        maze[r][c] = '.';
        visited[r][c] = 1;
        maze_print(maze);
        printf("\n\n%d %d\n\n", r, c);
        push_loc(&s1, r - 1, c);
        push_loc(&s1, r + 1, c);
        push_loc(&s1, r, c - 1);
        push_loc(&s1, r, c + 1);

        if (is_empty(&s1)) {
            printf("Fail\n");
            return 0;
        }
        else {
            element prev = here;
            here = pop(&s1);

            int dist = abs(here.r - prev.r) + abs(here.c - prev.c);

            if (dist == 1) {
                push(&s2, here);
                push(&s3, here);
            }

            else {
                    while (dist > 1) {
                        print_path(&s1);
                        print_path(&s2);
                        print_path(&s3);

                        element tmp = pop(&s2);
                        push(&s3, tmp);

                        prev = peek(&s2); 
                        dist = abs(here.r - prev.r) + abs(here.c - prev.c);
                    }
                push(&s3, here);
            }

        }
    }
    printf("Success\n");
    print_path(&s2);
    print_path(&s3);
    return 0;
}