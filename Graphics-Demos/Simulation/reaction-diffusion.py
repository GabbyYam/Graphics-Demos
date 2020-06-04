import taichi as ti
ti.init(arch = ti.gpu)

# Canvas
res = 512, 512

# Pixels set
matrix_a = ti.var(ti.f32, res)
matrix_b = ti.var(ti.f32, res)

# Rate of a & b
da = 1
db = 0.25

# Feed & kill rate 
f = 0.058
k = 0.062


# Laplacian filtering
@ti.func
def laplacian(i, j, matrix):
	val = matrix[i - 1, j - 1] * 0.05 + matrix[i - 1, j] * 0.2 + matrix[i - 1, j + 1] * 0.05\
		+ matrix[i, j - 1]     * 0.2  + matrix[i, j]     * -1  + matrix[i, j + 1] * 0.2\
		+ matrix[i + 1, j - 1] * 0.05 + matrix[i + 1, j] * 0.2 + matrix[i + 1, j + 1] * 0.05
	return val

# diffusion
@ti.func
def diffuse(i, j):
	matrix_a[i, j] += da * laplacian(i, j, matrix_a) - matrix_a[i, j] * matrix_b[i, j] ** 2 + f * (1 - matrix_a[i, j])
	matrix_b[i, j] += db * laplacian(i, j, matrix_b) + matrix_a[i, j] * matrix_b[i, j] ** 2 - (k + f) * matrix_b[i, j]

@ti.kernel
def render_and_swap():
	for i, j in ti.ndrange((1, res[0] - 1), (1, res[1] - 1)):
		diffuse(i, j)
	# for i, j in ti.ndrange((1, res[0] - 1), (1, res[1] - 1)):
	# 	t = matrix_a[i ,j]
	# 	matrix_a[i, j] = matrix_b[i, j]
	# 	matrix_b[i, j] = t

@ti.kernel
def fill_with_random():
	r = 5
	for k in range(20):
		x = int(res[0] * ti.random())
		y = int(res[1] * ti.random())
		for i, j in ti.ndrange((x - r, x + r), (y - r, y + r)):
			matrix_a[i, j] = da
			matrix_b[i, j] = db

@ti.kernel
def init_boader():
	for i, j in ti.ndrange((0, res[0]), (0, res[1])):
		if(i == 0 or j == 0 or i == res[0] - 1 or j == res[1] - 1):
				matrix_a[i, j] = 0
				matrix_b[i, j] = 0


gui = ti.GUI('Reaction Diffusion', res)

# init_boader()
fill_with_random()

while True:
	render_and_swap()
	gui.set_image(matrix_a)
	gui.show()
















