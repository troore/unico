
int main(int argc, char *argv[])
{
    while (scanf("%d %d", &n, &m) != EOF) {
        read();
        printf("%d\n", dinic());
		show();
    }
	
    return 0;
}
