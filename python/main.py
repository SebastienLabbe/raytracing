from curses import wrapper
import string as strings
import asyncio, traceback, time, pickle, os, curses, math, shutil
"""
GLOBAL VARIABLES/MACROS
"""

#class Global:
DATA_FOLDER = 'data'

HEIGHT = 0
WIDTH = 0
FRAME_COUNTER = 0
VISION = None
SCREEN_buff = [[]]
SCREEN_shown = [[]]
WORLD = None
MENU = None
FONT = None
STDSCR = None

COLISION = None
POINT = None

black = 0
red = 1
green = 2
yellow = 3
blue = 4
magenta = 5
cyan = 6
white = 7
COLORS = 8
colordict = {
        'black': 0,
        'red': 1,
        'green': 2,
        'yellow': 3,
        'blue': 4,
        'magenta': 5,
        'cyan': 6,
        'white': 7
        }


"""
CLASSES
"""

#attempt at creating a class to manage memory
#total fail better use cpp
class Temporary_manager:
    def __ini__(self, class_type, n = 3):
        self.temps = [class_type() for _ in range(n)]
        self.in_use = [False for _ in range(n)]


#the camera class which will take the picture and fill the buffer
class Vision:
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction
        self.right = Point(direction.y, -direction.x, direction.z)
        self.up = self.right * direction 
        self.distance_from_screen = 5
        self.pixels_per_unit = 10
        self.center = origin + direction ** self.distance_from_screen
        self.max_viewing_distance = 100
        self.colision_horizon = Colision(origin + direction ** self.max_viewing_distance)
        
        global HEIGHT, WIDTH
        self.rays = [[Line(self.origin, ((self.center 
            + self.right ** ((WIDTH // 2 + i) / self.pixels_per_unit) 
            + self.up ** (2 * ((HEIGHT // 2 + j) / self.pixels_per_unit)))
            - self.origin).normalized())
            for i in range( - WIDTH, 0)] 
            for j in range( - HEIGHT, 0)]

    #the functions that fills the buffer and sends the rays
    def write(self):
        global HEIGHT, WIDTH, SCREEN_buff, WORLD, COLISION, POINT
        chunk = Point(0, 0, 0)
        for j in range(HEIGHT):
            for i in range(WIDTH):
                COLISION.copy(self.colision_horizon)
                chunk.copy(self.rays[j][i].o)
                chunk.floordiv(WORLD.chunk_size)
                POINT.copy(self.rays[j][i].o)
                
                while not COLISION.found and chunk in WORLD.chunks:
                    
                    self.rays[j][i].hits_chunk(WORLD.chunks[chunk], COLISION)
                    x, y, z = POINT.tuple_form()
                    dx, dy, dz = self.rays[j][i].v.tuple_form()
                    if dx == 0:
                        rx = math.inf
                    elif dx > 0:
                        rx = (WORLD.chunk_size - (x % WORLD.chunk_size)) / dx
                    else:
                        rx = (( - x % WORLD.chunk_size) - WORLD.chunk_size) / dx
                    if dy == 0:
                        ry = math.inf
                    elif dy > 0:
                        ry = (WORLD.chunk_size - (y % WORLD.chunk_size)) / dy
                    else:
                        ry = (( - y % WORLD.chunk_size) - WORLD.chunk_size) / dy
                    if dz == 0:
                        rz = math.inf
                    elif dz > 0:
                        rz = (WORLD.chunk_size - (z % WORLD.chunk_size)) / dz
                    else:
                        rz = (( - z % WORLD.chunk_size) - WORLD.chunk_size) / dz
                    minn = min(rx, ry, rz)
                    POINT.x += minn * self.rays[j][i].v.x
                    POINT.y += minn * self.rays[j][i].v.y
                    POINT.z += minn * self.rays[j][i].v.z
                    chunk.copy(POINT)
                    chunk.floordiv(WORLD.chunk_size)
                
                if COLISION.found:
                    SCREEN_buff[j][i] = COLISION.color



#player class
class Player:
    def __init__(self, name):
        self.name = name
        self.warriors = []
        self.pos = Point(0, 0, 0)
        self.vel = Point(0, 0, 0)
        self.acc = Point(0, 0, 0)
        self.vis = Vision(self.pos, Point(1, 0, 0))

#base class for everyting it contains all the 3D geometric formulas between points
class Point:

    def __init__(self, x = 0, y = 0, z = 0):
        self.x = x
        self.y = y
        self.z = z
    
    def __add__(self, p):
        return Point(self.x + p.x, self.y + p.y, self.z + p.z)
    
    def __sub__(self, p):
        return Point(self.x - p.x, self.y - p.y, self.z - p.z)

    def __pow__(self, x):
        return Point(self.x * x, self.y * x, self.z * x)

    def __mul__(self, p): 
        return Point(self.y * p.z - self.z * p.y, self.z * p.x - self.x * p.z, self.x * p.y - self.y * p.x)

    def __truediv__(self, x):
        return Point(self.x / x, self.y / x, self.z / x)

    def __floordiv__(self, x):
        return Point(int(self.x // x), int(self.y // x), int(self.z // x))
    
    def __mod__(self, p):
        return self.x * p.x + self.y * p.y + self.z * p.z        

    def __getitem__(self, key):
        if key == 0:
            return self.x
        if key == 1:
            return self.y
        return self.z

    def __eq__(self, p):
        return (self.x == p.x and self.y == p.y and self.z == p.z)
    
    def __ifloordiv__(self, x):
        self.x //= x
        self.y //= x
        self.z //= x
    
    def floordiv(self, x):
        self.x //= x
        self.y //= x
        self.z //= x

    def distance(self, plane):
        return (self.x * plane.normal.x + self.y * plane.normal.y + self.z * plane.normal.z + plane.d) 
        # if it is not normalized then:  / plane.normal.norm()
    
    def norm(self):
        return math.sqrt( self.x * self.x + self.y * self.y + self.z * self.z)
    
    def copy(self, p):
        self.x = p.x
        self.y = p.y
        self.z = p.z

    def get_copy(self):
        return Point(self.x, self.y, self.z)

    def normalize(self):
        n = self.norm()
        self.x /= n
        self.y /= n
        self.z /= n
    
    def normalized(self):
        return self / self.norm()

    def dominant_axis(self):
        x = abs(self.x)
        y = abs(self.y)
        z = abs(self.z)
        maxx = max(x, y, z)
        if x == maxx:
            return [1, 2]
        if y == maxx:
            return [0, 2]
        return [0, 1]
    
    #chunk methods
    def __str__(self):
        return '{}_{}_{}'.format(self.x, self.y, self.z)

    def __hash__(self):
        return hash((self.x, self.y, self.z))

    def tuple_form(self):
        return (self.x, self.y, self.z)

#to store a colision like a point but with color normal to the colision plane ...
class Colision(Point):
    def __init__(self, point = None, color = None, found = False):
        if point == None:
            point = Point(0, 0, 0)
        super().__init__(point.x, point.y, point.z)
        self.color = color
        self.found = found

    def get_copy(self):
        return Colision(Point(self.x, self.y, self.z), self.color, self.found)

    def copy_with(self, colision, color = None, found = False):
        self.x = colision.x
        self.y = colision.y
        self.z = colision.z
        self.color = color
        self.found = found
    
    def copy(self, colision):
        self.x = colision.x
        self.y = colision.y
        self.z = colision.z
        self.color = None
        self.found = False

#line that will be used as a ray
class Line:
    def __init__(self, o, v):
        self.o = o
        self.v = v

    def get_max_view(self):
        return self.o + self.v ** Line.view_distance

    def hits_plane(self, plane, point):
        if plane.normal % self.v == 0:
            return False
        point.copy(self.o + (self.v ** (self.o.distance(plane) / (self.v % plane.normal))))
        return True

    def hits_triangle(self, tri, p):
        if not self.hits_plane(tri.plane, p):
            return False
        i1, i2 = tri.dominant_axis
        

        u0 = p[i1] - tri.v[0][i1]
        v0 = p[i2] - tri.v[0][i2]
        u1 = tri.v[1][i1] - tri.v[0][i1]
        v1 = tri.v[1][i2] - tri.v[0][i2]
        u2 = tri.v[2][i1] - tri.v[0][i1]
        v2 = tri.v[2][i2] - tri.v[0][i2]
        u3 = tri.v[2][i1] - tri.v[1][i1]
        v3 = tri.v[2][i2] - tri.v[1][i2]
        u4 = p[i1] - tri.v[1][i1]
        v4 = p[i2] - tri.v[1][i2]
        c1 = ((u1 * v0 - v1 * u0) > 0)
        c2 = (((u2 * v0 - v2 * u0) > 0) == c1)
        if c2:
            return False
        c3 = (((u3 * v4 - v3 * u4) > 0) != c1)
        if c3:
            return False
        return True

    def hits_chunk(self, chunk, colision):
        temp_colision = colision.get_copy()
        for object_ in chunk.objects:
            for triangle in object_.triangles:
                if self.hits_triangle(triangle, temp_colision):
                    if ((temp_colision - colision) % self.v) < 0 and ((temp_colision - self.o) % self.v) > 0:
                        colision.copy_with(temp_colision, object_.color, True)
        return colision.found

#plane to contain polygones and simplify colision checking
class Plane:
    def __init__(self, normal, d):
        self.normal = normal
        self.d = d

#to be thought best way to check colision decompose everything to triangles
class Triangle:
    def __init__(self, v):
        self.v = v
        
        self.normal_unit = self.get_normal_unit()
        d = self.normal_unit % v[0]
        
        self.plane = Plane(self.normal_unit, d)
        
        self.dominant_axis = self.normal_unit.dominant_axis()
        
        self.center = sum(self.v, Point(0,0,0)) / 3

    def get_normal_unit(self):
        return ((self.v[1] - self.v[0]) * (self.v[2] - self.v[0])).normalized()
    

#a list of triangle and a color
class Object:
    def __init__(self, triangles, color):
        self.triangles = triangles
        self.color = color
        self.center = sum([triangle.center for triangle in triangles], Point(0,0,0)) / len(triangles)

#to only load the neigboring objects
#and to only check ray colision with objects in the chunk it is in
#i would say this is the place to improve as it defintes the number of colision checks and makes a lot of unnecessary loops
class Chunk:
    def __init__(self, objects = []):
        self.objects = objects

    def add_object(self, object_):
        self.objects.append(object_)

#not sure what this does
class Vision:
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction
        self.right = Point(direction.y, -direction.x, direction.z)
        self.up = self.right * direction 
        self.distance_from_screen = 5
        self.pixels_per_unit = 10
        self.center = origin + direction ** self.distance_from_screen
        self.max_viewing_distance = 100
        self.colision_horizon = Colision(origin + direction ** self.max_viewing_distance)
        
        global HEIGHT, WIDTH
        self.rays = [[Line(self.origin, ((self.center 
            + self.right ** ((WIDTH // 2 + i) / self.pixels_per_unit) 
            + self.up ** (2 * ((HEIGHT // 2 + j) / self.pixels_per_unit)))
            - self.origin).normalized())
            for i in range( - WIDTH, 0)] 
            for j in range( - HEIGHT, 0)]

    def write(self):
        global HEIGHT, WIDTH, SCREEN_buff, WORLD
        colision = Colision(Point(0, 0, 0))
        chunk = Point(0, 0, 0)
        temp_origin = Point(0, 0, 0)
        for j in range(HEIGHT):
            for i in range(WIDTH):
                colision.copy(self.colision_horizon)
                chunk.copy(self.rays[j][i].o)
                chunk.floordiv(WORLD.chunk_size)
                temp_origin.copy(self.rays[j][i].o)
                
                while not colision.found and chunk in WORLD.chunks:
                    
                    self.rays[j][i].hits_chunk(WORLD.chunks[chunk], colision)
                    x, y, z = temp_origin.tuple_form()
                    dx, dy, dz = self.rays[j][i].v.tuple_form()
                    if dx == 0:
                        rx = math.inf
                    elif dx > 0:
                        rx = (WORLD.chunk_size - (x % WORLD.chunk_size)) / dx
                    else:
                        rx = (( - x % WORLD.chunk_size) - WORLD.chunk_size) / dx
                    if dy == 0:
                        ry = math.inf
                    elif dy > 0:
                        ry = (WORLD.chunk_size - (y % WORLD.chunk_size)) / dy
                    else:
                        ry = (( - y % WORLD.chunk_size) - WORLD.chunk_size) / dy
                    if dz == 0:
                        rz = math.inf
                    elif dz > 0:
                        rz = (WORLD.chunk_size - (z % WORLD.chunk_size)) / dz
                    else:
                        rz = (( - z % WORLD.chunk_size) - WORLD.chunk_size) / dz
                    minn = min(rx, ry, rz)
                    temp_origin.x += minn * self.rays[j][i].v.x
                    temp_origin.y += minn * self.rays[j][i].v.y
                    temp_origin.z += minn * self.rays[j][i].v.z
                    chunk.copy(temp_origin)
                    chunk.floordiv(WORLD.chunk_size)
                
                if colision.found:
                    SCREEN_buff[j][i] = colision.color


#again player confused????
class Player:
    def __init__(self, name):
        self.name = name
        self.warriors = []
        self.pos = Point(0, 0, 0)
        self.vel = Point(0, 0, 0)
        self.acc = Point(0, 0, 0)
        self.vis = Vision(self.pos, Point(1, 0, 0))
        

#world that stores all the objects in the right chunks
class World:
    def __init__(self, player, world_file = None):
        if world_file == None:
            self.chunks_rendered = 4
            self.chunks_padding = 2
            self.chunks_loaded = self.chunks_rendered + self.chunks_padding
            self.chunk_size = 100
            self.world_file = 'temp_save_world'
            try:
                shutil.rmtree('{}/{}/'.format(DATA_FOLDER,self.world_file))
            except:
                print('temp dir not found')
            os.makedirs('{}/{}/'.format(DATA_FOLDER,self.world_file))
        else:
            self.world_file = world_file
            self.load_world_info()
        
        self.player = player
        self.player.chunk = Point(player.pos.x, player.pos.y, player.pos.z) // self.chunk_size
        self.loaded_chunks = {
                Point(i, j, k)
                for i in range( - self.chunks_padding, self.chunks_padding + 1)
                for j in range( - self.chunks_padding, self.chunks_padding + 1)
                for k in range( - self.chunks_padding, self.chunks_padding + 1)
                }

        self.chunks = {}

        self.update_loaded_chunks()

    def update_loaded_chunks(self):
        current = set(self.chunks.keys())
        for chunk in current.difference(self.loaded_chunks):
            self.save_chunk(chunk)
            del self.chunks[chunk]
        for chunk in self.loaded_chunks.difference(current):
            self.load_chunk(chunk)

    def load_world_info(self):
        return

    def chunk_filename(self, chunk):
        return '{}/{}/chunk_{}.pkl'.format(DATA_FOLDER, self.world_file, chunk)

    def load_chunk(self, chunk):
        try:
            with open(self.chunk_filename(chunk), 'rb') as file:
                self.chunks[chunk] = pickle.load(file)
        except Exception as e:
            #traceback.print_exc()
            self.chunks[chunk] = Chunk()
            #self.save_chunk(chunk)

    def save_chunk(self, chunk):
        with open(self.chunk_filename(chunk), 'wb') as file:
            pickle.dump(self.chunks[chunk], file)

    def add_object(self, object_):
        chunk = object_.center // self.chunk_size
        if chunk in self.chunks:
            self.chunks[chunk].add_object(object_)
        else:
            self.load_chunk(chunk)
            self.chunks[chunk].add_object(object_)
            self.save_chunk(chunk)


#clearly small class that was to become the textures
class Pixmap:
    def __init__(self):
        return





"""
FUNCTIONS
"""

def init_color():
    curses.start_color()
    for i1 in range(COLORS): 
        for i2 in range(COLORS):
            curses.init_pair(1+i1*COLORS+i2, i1, i2)

def cpair(c):
    return curses.color_pair(1+c[0]*COLORS+c[1])

def write_screen():
    global STDSCR, FRAME_COUNTER
    FRAME_COUNTER += 1
    for i in range(HEIGHT):
        for j in range(WIDTH):
            if i == 0 and j == 0:
                STDSCR.addstr(0, 0, str(FRAME_COUNTER), cpair([white, SCREEN_shown[i][j]]))
                continue
            STDSCR.addstr(i, j, ' ', cpair([black, SCREEN_shown[i][j]]))
    STDSCR.refresh()

def wipe_screen(screen):
    global WIDTH, HEIGHT
    for i in range(WIDTH):
        for j in range(HEIGHT):
            screen[j][i] = black


"""
GAME ASYNC MANAGEMENT
"""

async def manage_input():
    global STDSCR
    char = STDSCR.getch()
    if char == 259:
        #up arrow
        pass
    elif char == 258:
        #down arrow
        pass
    elif char == 10:
        #enter
        pass
    elif char != -1:
        print(char)


async def screen_loop():
    write_screen() 

async def game_loop():
    global SCREEN_buff, VISION
    wipe_screen(SCREEN_buff)
    VISION.write()

async def main_loop():
    global SCREEN_buff, SCREEN_shown
    nframes = 100
    fps = 0
    x = 0
    while 1:
        start = time.time()
        for i in range(nframes):
            loop_game = loop.create_task(game_loop())
            loop_screen = loop.create_task(screen_loop())
            input_manage = loop.create_task(manage_input())
            await asyncio.wait([loop_game, loop_screen, input_manage])
            SCREEN_buff, SCREEN_shown = (SCREEN_shown, SCREEN_buff)
            time.sleep(x)
        fps = nframes / (time.time() - start)

def create_cube(center, size):
    corners = [Point(center.x + i * size, center.y + j * size, center.z + k * size) for i in (1, -1) for j in (1, -1) for k in (1, -1)]
    triangles = [Triangle(corners[i], corners[j], corners[k]) for i in range(len(corners)) for j in range(i+1, len(corners)) for k in range(j+1, len(corners))]
    cube = Object(triangles, green)
    return cube

def create_test_tri(v1, v2, v3, color):
    return Object([Triangle([Point(v1[0], v1[1], v1[2]),Point(v2[0], v2[1], v2[2]),Point(v3[0], v3[1], v3[2])])], color)

async def main():
    global SCREEN_buff, SCREEN_shown, HEIGHT, WIDTH, STDSCR, WORLD, VISION, COLISION, POINT
    
    #CURSES INIT
    STDSCR = curses.initscr()
    curses.noecho()
    curses.cbreak()
    STDSCR.keypad(True)
    STDSCR.nodelay(True)
    STDSCR.clear()
    init_color()

    HEIGHT, WIDTH = STDSCR.getmaxyx()
    WIDTH -= 1
    
    #BUFFERS CREATION
    SCREEN_buff = [[black for i in range(WIDTH)] for j in range(HEIGHT)]
    SCREEN_shown = [[black for i in range(WIDTH)] for j in range(HEIGHT)]
    COLISION = Colision(0, 0, 0)
    POINT = Point(0, 0, 0)
 
    #WORLD CREATION
    player = Player('Seb')
    VISION = player.vis
    WORLD = World(player)
    #cube = create_cube(Point(10,0,0), 1)
    for t1, t2, t3, color in [
            [[10,5,5],[10,0,0],[10,5,-5],green], 
            [[10,5,-5],[10,0,0],[10,-5,-5],red], 
            [[10,-5,-5],[10,0,0],[10,-5,5],yellow], 
            [[10,-5,5],[10,0,0],[10,5,5],blue]]:
        test_tri = create_test_tri(t1, t2, t3, color)
        WORLD.add_object(test_tri)
    

    #LOOP
    loop_main = loop.create_task(main_loop())
    await asyncio.wait([loop_main])


"""
PROGRAM
"""

if __name__ == "__main__":
    try:
        loop = asyncio.get_event_loop()
        loop.run_until_complete(main())
    except Exception as e:
        traceback.print_exc()
        time.sleep(11)
    finally:
        loop.close()
        curses.nocbreak()
        STDSCR.keypad(False)
        curses.echo()
        curses.endwin()
