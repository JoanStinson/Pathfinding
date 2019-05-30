#include "Agent.h"

Agent::Agent() : sprite_texture(0),
                 position(Vector2D(100, 100)),
	             target(Vector2D(1000, 100)),
	             velocity(Vector2D(0,0)),
	             mass(0.1f),
	             max_force(150),
	             max_velocity(200),
	             orientation(0),
	             color({ 255,255,255,255 }),
				 sprite_num_frames(0),
	             sprite_w(0),
	             sprite_h(0),
	             draw_sprite(false)
{
	steering_behavior = new SteeringBehavior; 
}

Agent::~Agent() {
	if (sprite_texture)
		SDL_DestroyTexture(sprite_texture);
	if (steering_behavior)
		delete (steering_behavior);
}

float Agent::RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

float Agent::Heuristic(Vector2D a, Vector2D b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

void Agent::PrintStatistics(int a) {
	// Current
	current = a;

	// Min
	if (min == 0) min = a;
	else if (a < min) min = a;

	// Max
	if (max == 0) max = a;
	else if (a > max) max = a;

	// Average
	sizes.push_back(a);
	int temp = 0;
	for (int i = 0; i < sizes.size(); i++) {
		temp += sizes[i];
	}
	average = temp / sizes.size();

	// Print
	cout << "\r" << "  Current: " << current << " Min: " << min << " Max: " << max << " Average: " << average << "            ";
}

vector<Vector2D> Agent::BFS(Vector2D start, Vector2D goal, Graph graph) {
	// Creem la frontera on emmagetzarem tots els nodes que visitem 
	// i la inicialitzem amb la posició del player = start
	queue<Vector2D> frontier;
	frontier.push(start);
	frontierCount.clear();

	// Creem l'estructura came_from la qual determina el node anterior del que proveniem per traçar el camí 
	// i com la posició del player = start no provenia de res = NULL ja que aquesta és la posició inicial
	unordered_map<Vector2D, Vector2D> came_from;
	came_from[start] = NULL;

	vector<Vector2D> path, neighbors;
	Vector2D current, next;
	bool visited;

	// Mentre la frontera no estigui buida, és a dir, mentre faltin nodes per explorar
	while (!frontier.empty()) {

		// El node actual del qual visitarem els veins es el primer node que hem afegit o el node més antic
		// i fem pop ja que en l'iteració anterior ja hem fet servir aquest node
		current = frontier.front();
		frontier.pop();

		// Si el node actual és el node goal, és a dir, el node de la moneda
		// ja hem acabat de visitar tots els nodes necessaris i ens decidim a retornar el camí per arribar-hi (early exit)
		if (current == goal) {
			int size = frontierCount.size();
			PrintStatistics(size);
			// Fem push_back del current el qual = node final o goal 
			// (perquè volem traçar el camí desde el final al principi)
			path.push_back(current);
			// Mentre no agafem el node incial
			while (current != start) {
				// Anem afegint els nodes que hem visitat abans del node final
				current = came_from[current];
				path.push_back(current);
			}
			// Com que no hem agafat el node incial ja que la condició no ens ho permetia 
			// (perquè abans del start no hi ha més nodes) li fem un push del start
			path.push_back(start);
			// I com hem traçat el camí del final al principi fem un reverse
			// perquè el personatge comenci del node inicial i vagi fins el node final
			std::reverse(path.begin(), path.end());
			// Per tant, retornem el camí, seria molt semblant a fer break
			return path;
		}

		// En cas que no haguem trobat el node final, agafem els veins de 1-4 del current
		neighbors = graph.GetConnections(current);

		// Iterem sobre aquest i anem agafant-los un a un
		for (unsigned int i = 0; i < neighbors.size(); i++) {
			visited = false;
			next = neighbors[i];

			// Per cadascun determinem si l'hem visitat o no
			//for (unsigned int j = 0; j < came_from.size(); j++) {
				if (came_from.find(next) != came_from.end()) { // 'if (came_from.count(next))' és el mateix per mirar si existeix la key next, però al ser unordered es + lent
					visited = true;
				}
			//}
			// Si no l'hem visitat l'afegim a la frontier
			// i agafem el node current per traçar el camí 
			// (ja que no podem traçar un camí a base de veïns)
			if (!visited) {
				frontier.push(next);
				came_from[next] = current;
				//Count :)
				frontierCount.push_back(next);
			}
		}
	}
}

vector<Vector2D> Agent::Dijkstra(Vector2D start, Vector2D goal, Graph graph) {
	PriorityQueue<Vector2D, float> frontier;
	frontier.put(start, 0.f);
	frontierCount.clear();
	vector_costs.clear();

	unordered_map<Vector2D, Vector2D> came_from;
	came_from[start] = NULL;

	unordered_map<Vector2D, float> cost_so_far;
	cost_so_far[start] = 0.f;

	vector<Vector2D> path, neighbors;
	Vector2D current, next;
	bool visited;
	float new_cost, priority;
	float randCost;

	while (!frontier.empty()) {

		current = frontier.get();

		if (current == goal) {
			int size = frontierCount.size();
			PrintStatistics(size);
			path.push_back(current);
			while (current != start) {
				current = came_from[current];
				path.push_back(current);
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}

		neighbors = graph.GetConnections(current);
		
		
		
		//cout << randCost << endl;
		for (unsigned int i = 0; i < neighbors.size(); i++) {
			visited = false;
			next = neighbors[i];
			randCost = (rand() % 6) + 1; //RandomFloat(0, 7);
			new_cost = cost_so_far[current] + randCost;//(rand() % 3) + 1;//graph.GetCost(next);
			

			//for (unsigned int j = 0; j < cost_so_far.size(); j++) {
				// If next in cost_so_far 
				if (cost_so_far.find(next) != cost_so_far.end()) { 
					if (new_cost > cost_so_far[next]) { // if 'new_cost < cost_so_far[next]' visited = false perque el volem afegir, if 'new_cost > cost_so_far[next]' nol volem per tant visited = false
						visited = true;
					}
				}
				// If next not in cost_so_far
				else { 
					visited = false;
				}

			//}

			if (!visited) {
				cost_so_far[next] = new_cost;
				priority = new_cost;
				frontier.put(next, priority);
				came_from[next] = current;
				//Count :)
				frontierCount.push_back(next);
				vector_costs.push_back(std::make_pair(next, randCost));
			}
		}

		
	}
}

vector<Vector2D> Agent::GBFS(Vector2D start, Vector2D goal, Graph graph) {
	PriorityQueue<Vector2D, float> frontier;
	frontier.put(start, 0.f);
	frontierCount.clear();

	unordered_map<Vector2D, Vector2D> came_from;
	came_from[start] = NULL;

	vector<Vector2D> path, neighbors;
	Vector2D current, next;
	bool visited;
	float priority;
	while (!frontier.empty()) {

		current = frontier.get();

		if (current == goal) {
			int size = frontierCount.size();
			PrintStatistics(size);
			path.push_back(current);
			while (current != start) {
				current = came_from[current];
				path.push_back(current);
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}

		neighbors = graph.GetConnections(current);

		for (unsigned int i = 0; i < neighbors.size(); i++) {
			visited = false;
			next = neighbors[i];

			//for (unsigned int j = 0; j < came_from.size(); j++) {
				if (came_from.find(next) != came_from.end()) { 
					visited = true;
				}
			//}

			if (!visited) {
				priority = Heuristic(goal, next);
				frontier.put(next, priority);
				came_from[next] = current;
				//Count :)
				frontierCount.push_back(next);
			}
		}
	}
}

vector<Vector2D> Agent::AStar(Vector2D start, Vector2D goal, Graph graph, bool show_nodes) {
	PriorityQueue<Vector2D, float> frontier;
	frontier.put(start, 0.f);

	

	unordered_map<Vector2D, Vector2D> came_from;
	came_from[start] = NULL;

	unordered_map<Vector2D, float> cost_so_far;
	cost_so_far[start] = 0.f;

	vector<Vector2D> path, neighbors;
	Vector2D current, next;
	bool visited;
	float new_cost, priority;
	float randCost;

	while (!frontier.empty()) {

		current = frontier.get();

		if (current == goal) {
			if (show_nodes) {
				int size = frontierCount.size();
				PrintStatistics(size);
			}
			else cout << "\r" << "           " << "    " << "      " << "    " << "      " << "    " << "          " << "    " << "            ";
			path.push_back(current);
			while (current != start) {
				current = came_from[current];
				path.push_back(current);
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}

		neighbors = graph.GetConnections(current);

		

		for (unsigned int i = 0; i < neighbors.size(); i++) {
			visited = false;
			next = neighbors[i];
			randCost = (rand() % 6) + 1; //RandomFloat(0, 7);
			new_cost = cost_so_far[current] + randCost; //TODO implement GetCost method to do + 'graph.GetCost(current, next)' instead of rand
			
			//for (unsigned int j = 0; j < cost_so_far.size(); j++) {
				// If next in cost_so_far 
				if (cost_so_far.find(next) != cost_so_far.end()) {
					if (new_cost > cost_so_far[next]) { // if 'new_cost < cost_so_far[next]' visited = false perque el volem afegir, if 'new_cost > cost_so_far[next]' nol volem per tant visited = false
						visited = true;
					}
				}
				// If next not in cost_so_far
				else {
					visited = false;
				}

			//}

			if (!visited) {
				cost_so_far[next] = new_cost;
				priority = new_cost + Heuristic(goal, next);
				frontier.put(next, priority);
				came_from[next] = current;
				//Count :)
				frontierCount.push_back(next);
				vector_costs.push_back(std::make_pair(next, randCost));
			}
		}
	}
}

vector<Vector2D> Agent::AStar2(Vector2D start, Vector2D goal, Graph graph, bool show_nodes, vector<Vector2D> enemies) {
	PriorityQueue<Vector2D, float> frontier;
	frontier.put(start, 0.f);
	frontierCount.clear();
	vector_costs.clear();


	unordered_map<Vector2D, Vector2D> came_from;
	came_from[start] = NULL;

	unordered_map<Vector2D, float> cost_so_far;
	cost_so_far[start] = 0.f;

	vector<Vector2D> path, neighbors;
	Vector2D current, next;
	bool visited;
	float new_cost, priority;
	float randCost;

	while (!frontier.empty()) {

		current = frontier.get();

		if (current == goal) {
			if (show_nodes) {
				int size = frontierCount.size();
				PrintStatistics(size);
			}
			else cout << "\r" << "           " << "    " << "      " << "    " << "      " << "    " << "          " << "    " << "            ";
			path.push_back(current);
			while (current != start) {
				current = came_from[current];
				path.push_back(current);
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}

		neighbors = graph.GetConnections(current);

		

		for (unsigned int i = 0; i < neighbors.size(); i++) {
			visited = false;
			next = neighbors[i];
			randCost = (rand() % 6) + 1; //RandomFloat(0, 7);
			if (std::find(enemies.begin(), enemies.end(), next) != enemies.end()) {
				randCost = 99;
			}

			new_cost = cost_so_far[current] + randCost; //TODO implement GetCost method to do + 'graph.GetCost(current, next)' instead of rand

			//for (unsigned int j = 0; j < cost_so_far.size(); j++) {
				// If next in cost_so_far 
				if (cost_so_far.find(next) != cost_so_far.end()) {
					if (new_cost > cost_so_far[next]) { // if 'new_cost < cost_so_far[next]' visited = false perque el volem afegir, if 'new_cost > cost_so_far[next]' nol volem per tant visited = false
						visited = true;
					}
				}
				// If next not in cost_so_far
				else {
					visited = false;
				}

			//}

			if (!visited) {
				cost_so_far[next] = new_cost;
				priority = new_cost + Heuristic(goal, next);
				frontier.put(next, priority);
				came_from[next] = current;
				//Count :)
				frontierCount.push_back(next);
				vector_costs.push_back(std::make_pair(next, randCost));
			}
		}
	}
}

SteeringBehavior * Agent::Behavior() {
	return steering_behavior;
}

Vector2D Agent::getPosition() {
	return position;
}

Vector2D Agent::getTarget() {
	return target;
}

Vector2D Agent::getVelocity() {
	return velocity;
}

float Agent::getMaxVelocity() {
	return max_velocity;
}

void Agent::setPosition(Vector2D _position) {
	position = _position;
}

void Agent::setTarget(Vector2D _target) {
	target = _target;
}

void Agent::setVelocity(Vector2D _velocity) {
	velocity = _velocity;
}

void Agent::setMass(float _mass) {
	mass = _mass;
}

void Agent::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	color = { r, g, b, a };
}

void Agent::update(Vector2D steering_force, float dtime, SDL_Event *event) {

	switch (event->type) {
		/* Keyboard & Mouse events */
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_sprite = !draw_sprite;
		break;
	default:
		break;
	}


	Vector2D acceleration = steering_force / mass;
	velocity = velocity + acceleration * dtime;
	velocity = velocity.Truncate(max_velocity);

	position = position + velocity * dtime;


	// Update orientation
	if (velocity.Length()>0)
		orientation = (float)(atan2(velocity.y, velocity.x) * RAD2DEG);


	// Trim position values to window size
	if (position.x < 0) position.x = TheApp::Instance()->getWinSize().x;
	if (position.y < 0) position.y = TheApp::Instance()->getWinSize().y;
	if (position.x > TheApp::Instance()->getWinSize().x) position.x = 0;
	if (position.y > TheApp::Instance()->getWinSize().y) position.y = 0;
}

void Agent::draw() {
	if (draw_sprite) {
		Uint32 sprite;
		
		if (velocity.Length() < 5.0)
			sprite = 1;
		else
			sprite = (int)(SDL_GetTicks() / (max_velocity)) % sprite_num_frames;
		
		SDL_Rect srcrect = { (int)sprite * sprite_w, 0, sprite_w, sprite_h };
		SDL_Rect dstrect = { (int)position.x - (sprite_w / 2), (int)position.y - (sprite_h / 2), sprite_w, sprite_h };
		SDL_Point center = { sprite_w / 2, sprite_h / 2 };
		SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), sprite_texture, &srcrect, &dstrect, orientation+90, &center, SDL_FLIP_NONE);
	}
	else {
		draw_circle(TheApp::Instance()->getRenderer(), (int)position.x, (int)position.y, 15, color.r, color.g, color.b, color.a);
		SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)position.x, (int)position.y, (int)(position.x+15*cos(orientation*DEG2RAD)), (int)(position.y+15*sin(orientation*DEG2RAD)));
	}
}

bool Agent::loadSpriteTexture(char* filename, int _num_frames) {
	if (_num_frames < 1) return false;

	SDL_Surface *image = IMG_Load(filename);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	sprite_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	sprite_num_frames = _num_frames;
	sprite_w = image->w / sprite_num_frames;
	sprite_h = image->h;
	draw_sprite = true;

	if (image)
		SDL_FreeSurface(image);

	return true;
}