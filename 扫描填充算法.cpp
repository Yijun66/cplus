
#include <stdio.h>
#include <glut.h>
static int n;
static int y_max;
static int x_max;
typedef struct Point {
	int x;
	int y;
} Point1;
static Point1 seed;
Point *InitPoint() {
	n = 8;
	Point1 *tran = (Point*)malloc(n * sizeof(struct Point));
	tran[0].x = 10; tran[0].y = 10;
	tran[1].x = 90; tran[1].y = 10;
	tran[2].x = 90; tran[2].y = 60;
	tran[3].x = 140; tran[3].y = 60;
	tran[4].x = 140; tran[4].y = 160;
	tran[5].x = 80; tran[5].y = 160;
	tran[6].x = 80; tran[6].y = 70;
	tran[7].x = 10; tran[7].y = 70;
	seed.x = 50; seed.y = 50;
	return tran;
}

void setEdge(int x, int y, int **NumGroup) {
	NumGroup[x][y] = 1;
}
void MidpointLine(int x0, int y0, int x1, int y1, int **NumGroup)
{
	int a, b, d1, d2, d, x, y; float m;
	if (x1 < x0) {
		d = x0, x0 = x1, x1 = d;
		d = y0, y0 = y1, y1 = d;
	}
	a = y0 - y1, b = x1 - x0;
	if (b == 0)
		m = -1 * a * 100;
	else
		m = (float)a / (x0 - x1); x = x0, y = y0;
	setEdge(x, y, NumGroup);
	if (m >= 0 && m <= 1) {
		d = 2 * a + b; d1 = 2 * a, d2 = 2 * (a + b);
		while (x < x1) {
			if (d <= 0) {
				x++, y++, d += d2;
			}
			else {
				x++, d += d1;
			}
			setEdge(x, y, NumGroup);
		}
	}
	else if (m <= 0 && m >= -1) {
		d = 2 * a - b; d1 = 2 * a - 2 * b, d2 = 2 * a;
		while (x < x1) {
			if (d > 0) { x++, y--, d += d1; }
			else {
				x++, d += d2;
			}
			setEdge(x, y, NumGroup);
		}
	}
	else if (m > 1) {
		d = a + 2 * b; d1 = 2 * (a + b), d2 = 2 * b;
		while (y < y1) {
			if (d > 0) {
				x++, y++, d += d1;
			}
			else {
				y++, d += d2;
			}
			setEdge(x, y, NumGroup);
		}
	}
	else {
		d = a - 2 * b; d1 = -2 * b, d2 = 2 * (a - b);
		while (y > y1) {
			if (d <= 0) {
				x++, y--, d += d2;
			}
			else {
				y--, d += d1;
			}
			setEdge(x, y, NumGroup);
		}
	}
}

void print(int **NumGroupMaxtrx, int x_num, int y_num) {
	int i;
	int j;
	for (i = 0; i < x_num; i++) {
		for (j = 0; j < y_num; j++) {
			if (NumGroupMaxtrx[i][j] == 1) {
				glVertex2i(i, j);
			}
		}

	}
}
int **InitPointMatrixByPoint(Point *p) {
	int i;
	y_max = p[0].x;
	x_max = p[0].y;
	for (i = 0; i < n; i++) {
		if (p[i].x > x_max)
			x_max = p[i].x;
		if (p[i].y > y_max)
			y_max = p[i].y;
	}
	y_max++; x_max++;
	int **NumGroup_Matrix = (int**)malloc(x_max * sizeof(int *));
	for (i = 0; i < x_max; i++) {
		NumGroup_Matrix[i] = (int*)malloc(y_max * sizeof(int));
	}
	int j;
	for (i = 0; i < x_max; i++) {
		for (j = 0; j < y_max; j++) {
			NumGroup_Matrix[i][j] = -1;
		}
	}
	for (i = 0; i < n; i++) {
		if (i != n - 1)
			MidpointLine(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y, NumGroup_Matrix);
		else
			MidpointLine(p[i].x, p[i].y, p[0].x, p[0].y, NumGroup_Matrix);
	}
	return NumGroup_Matrix;
}
struct STACKNODE {
	Point point;
	struct STACKNODE *next;
};
typedef struct STACKNODE *PtrToNode;
typedef struct STACKNODE *Stack;
Stack createStack() {
	Stack stack = (STACKNODE*)malloc(sizeof(struct STACKNODE));
	stack->next = NULL;
	return stack;
}
void *Push(Stack stack, Point point) {
	PtrToNode tempNode = (STACKNODE*)malloc(sizeof(struct STACKNODE));
	tempNode->point.x = point.x;
	tempNode->point.y = point.y;
	tempNode->next = stack->next;
	stack->next = tempNode;
	return 0;
}
Point *PopAndTop(Stack stack) {
	PtrToNode ptr = stack->next;
	stack->next = stack->next->next;
	return &ptr->point;
}
int IsNull(Stack s) {
	if (s->next == NULL)
		return 1;
	else
		return 0;
}
void scanLineFixArea(int **numGroupMatrix) {
	Stack s = createStack();
	Push(s, seed);
	Point *tempPoint;
	Point left, right;
	int i;
	while (!IsNull(s)) {
		tempPoint = PopAndTop(s);
		glVertex2i(tempPoint->x, tempPoint->y); numGroupMatrix[tempPoint->x][tempPoint->y] = 0;
		left.y = tempPoint->y;
		right.y = tempPoint->y;
		left.x = tempPoint->x;
		right.x = tempPoint->x;
		while (numGroupMatrix[left.x][left.y] != 1) {
			glVertex2i(left.x, left.y); numGroupMatrix[left.x][left.y] = 0;
			left.x--;
		}
		while (numGroupMatrix[right.x][right.y] != 1) {
			glVertex2i(right.x, right.y); numGroupMatrix[right.x][right.y] = 0;
			right.x++;
		}
		for (i = right.x; i >= left.x; i--) {
			if (numGroupMatrix[i][right.y + 1] == -1) {
				right.y++;
				right.x = i;
				Push(s, right);
				break;
			}
		}
		right.y = tempPoint->y;
		for (i = right.x; i >= left.x; i--) {
			if (numGroupMatrix[i][right.y - 1] == -1) {
				right.y--;
				right.x = i;
				Push(s, right);
				break;
			}

		}
	}
}
void ProcessExcute() {
	Point *p = InitPoint();
	int **numGroupMatrix = InitPointMatrixByPoint(p);
	scanLineFixArea(numGroupMatrix);
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	ProcessExcute();
	glEnd();
	glFlush();;
}
void Init() {
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 240, 0, 240);
}
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(400, 400);
	glutCreateWindow("…®√Ëœﬂ«¯”ÚÃÓ≥‰À„∑®");
	Init();
	glutDisplayFunc(display);
	glutMainLoop();
	system("pause");
	return 0;

}
