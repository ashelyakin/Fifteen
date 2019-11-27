#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<set>
#include<stack>
#include<queue>

using namespace std;

struct Node
{
	int rating;
	int depth;
	vector<int> state;
	Node* pred;

	friend bool operator<(const Node& const n1, const Node& const n2);

	~Node()
	{
		state.clear();
	}
};

bool operator<(const Node& const lhs, const Node& const rhs)
{
	bool f = 0;
	for (int i = 0; i < 16; i++)
	{
		if (lhs.state[i] != rhs.state[i])
		{
			f = 1;
			break;
		}
	}

	if (f == 0)
	{
		return false;
	}

	if (lhs.rating != rhs.rating)
		return lhs.rating < rhs.rating;

	for (int i = 0; i < 16; i++)
	{
		if (lhs.state[i] != rhs.state[i])
			return lhs.state[i] < rhs.state[i];
	}
	return false;
}

struct comparator
{
	bool operator()(const Node& a, const Node& b)
	{
		return (a.rating > b.rating);
	}
};

vector<int> from_text_to_state(string text)
{
	if (text.length() != 16)
		throw exception("Wrong length input text");


	vector<int> result;
	result.resize(16);

	for (int i = 0; i < 16; i++)
	{
		if (isdigit(text[i]))
		{
			result[i] = text[i] - 48;
		}
		else
		{
			result[i] = text[i] - 55;
		}
	}
	return result;
}

int manhattan_distance(const vector<int> & input)
{
	int result = 0;

	for (int i = 0; i < 16; i++)
	{
		if (input[i] != 0)
			result += abs(i / 4 - (input[i] - 1) / 4) + abs(i % 4 - (input[i] - 1) % 4);
	}
	return result;
}

bool check_feasibility(const vector<int>& input)
{
	int counter = 0;
	int ind;

	for (int i = 0; i < 16; i++)
		if (input[i] == 0)
			ind = i;

	for (int i = 0; i < 15; i++)
	{
		if (input[i] == 0)
			continue;
		for (int j = i; j < 16; j++)
		{
			if (input[j] == 0)
				continue;

			if (input[i] > input[j])
				counter++;
		}
	}

	counter += (ind / 4) + 1;

	if (counter % 2 == 0)
		return true;
	else
		return false;
}

void push_in_queue(Node n, priority_queue<Node, vector<Node>, comparator>& q, set<Node>& s)
{
	if (check_feasibility(n.state))
	{
		auto it = s.find(n);
		if (s.find(n) == s.end())
			q.push(n);
		else
		{
			if (it->depth > n.depth)
			{
				s.erase(it);
				s.insert(n);
				q.push(n);
			}
		}
	}
}

void fifteen_A(string text)
{

	vector<int> states = from_text_to_state(text);

	if (!check_feasibility(states))
	{
		cout << "impossible" << endl;
		return;
	}

	Node* start = new Node({ manhattan_distance(states), 0, states, nullptr });


	priority_queue<Node, vector<Node>, comparator> q;

	q.push(*start);

	set<Node> s;

	s.insert(*start);

	while (true)
	{
		if (q.empty())
		{
			cout << "fail" << endl;
			return;
		}

		Node* current_n = new Node(q.top());
		q.pop();

		if (current_n->state == vector<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 }))
		{
			int depth = current_n->depth;

			stack<vector<int>> out;
			while (current_n != nullptr)
			{
				out.push(current_n->state);

				current_n = current_n->pred;
			}

			while (!out.empty())
			{
				vector<int> x = out.top();
				out.pop();

				for (int i = 0; i < 16; i++)
					cout << x[i] << ' ';
				cout << endl;
			}
			
			cout << endl << "depth: " << depth << endl;

			return;
		}

		if (current_n->depth > 80)
			continue;

		int ind;

		for (int i = 0; i < 16; i++)
			if (current_n->state[i] == 0)
			{
				ind = i;
				break;
			}

		Node* newNode = new Node();

		newNode->depth = current_n->depth + 1;
		newNode->pred = current_n;
		newNode->state.resize(16);

		for (int i = 0; i < 16; i++)
			newNode->state[i] = current_n->state[i];

		//сверху вниз
		if (ind > 3)
		{
			newNode->state[ind] = current_n->state[ind - 4];
			newNode->state[ind - 4] = current_n->state[ind];
			newNode->rating = manhattan_distance(newNode->state) + newNode->depth;
			
			push_in_queue(*newNode, q, s);

			newNode->state[ind] = current_n->state[ind];
			newNode->state[ind - 4] = current_n->state[ind - 4];
		}

		//снизу вверх
		if (ind < 12)
		{
			newNode->state[ind] = current_n->state[ind + 4];
			newNode->state[ind + 4] = current_n->state[ind];
			newNode->rating = manhattan_distance(newNode->state) + newNode->depth;

			push_in_queue(*newNode, q, s);

			newNode->state[ind] = current_n->state[ind];
			newNode->state[ind + 4] = current_n->state[ind + 4];

		}

		//слева направо
		if (ind % 4 > 0)
		{
			newNode->state[ind] = current_n->state[ind - 1];
			newNode->state[ind - 1] = current_n->state[ind];
			newNode->rating = manhattan_distance(newNode->state) + newNode->depth;

			push_in_queue(*newNode, q, s);

			newNode->state[ind] = current_n->state[ind];
			newNode->state[ind - 1] = current_n->state[ind - 1];
		}

		//справа налево
		if (ind % 4 < 3)
		{
			newNode->state[ind] = current_n->state[ind +1];
			newNode->state[ind +1] = current_n->state[ind];
			newNode->rating = manhattan_distance(newNode->state) + newNode->depth;

			push_in_queue(*newNode, q, s);

			newNode->state[ind] = current_n->state[ind];
			newNode->state[ind +1] = current_n->state[ind +1];
		}

	}
}

struct ToClose
{
	vector<int> states;

	friend bool operator<(const ToClose& const lhs, const ToClose& const rhs)
	{
		for (int i = 0; i < 16; i++)
		{
			if (lhs.states[i] != rhs.states[i])
				return lhs.states[i] < rhs.states[i];
		}

		return false;
	}
};

void fifteen_IDA(string text)
{
	vector<int> states = from_text_to_state(text);

	if (!check_feasibility(states))
	{
		cout << "impossible" << endl;
		return;
	}

	Node* start = new Node({ manhattan_distance(states), 0, states, nullptr });

	set<Node> q;
	set<ToClose> close;

	q.insert(*start);

	while (true)
	{
		if (q.empty())
		{
			cout << "fail" << endl;
			return;
		}

		Node* current_n = new Node(*q.begin());
		q.erase(*current_n);
		close.insert(ToClose({ current_n->state }));

		if (current_n->state == vector<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 }))
		{
			int depth = current_n->depth;

			stack<vector<int>> out;
			while (current_n != nullptr)
			{
				out.push(current_n->state);

				current_n = current_n->pred;
			}

			while (!out.empty())
			{
				vector<int> x = out.top();
				out.pop();

				for (int i = 0; i < 16; i++)
					cout << x[i] << ' ';
				cout << endl;
			}

			cout << endl << "depth: " << depth << endl;

			return;
		}

		if (current_n->depth > 80)
			continue;

		int ind;

		for (int i = 0; i < 16; i++)
			if (current_n->state[i] == 0)
			{
				ind = i;
				break;
			}


		//сверху вниз
		if (ind > 3)
		{
			Node* newNode = new Node();

			newNode->depth = current_n->depth + 1;
			newNode->pred = current_n;
			newNode->state.resize(16);

			for (int i = 0; i < 16; i++)
				newNode->state[i] = current_n->state[i];

			newNode->state[ind] = current_n->state[ind - 4];
			newNode->state[ind - 4] = current_n->state[ind];

			if (close.count(ToClose({ newNode->state })) == 0)
			{
				newNode->rating = manhattan_distance(newNode->state) + newNode->depth;
				q.insert(*newNode);
			}

			newNode->state[ind] = current_n->state[ind];
			newNode->state[ind - 4] = current_n->state[ind - 4];
		}

		//снизу вверх
		if (ind < 12)
		{
			Node* newNode = new Node();

			newNode->depth = current_n->depth + 1;
			newNode->pred = current_n;
			newNode->state.resize(16);

			for (int i = 0; i < 16; i++)
				newNode->state[i] = current_n->state[i];

			newNode->state[ind] = current_n->state[ind + 4];
			newNode->state[ind + 4] = current_n->state[ind];

			if (close.count(ToClose({ newNode->state })) == 0)
			{
				newNode->rating = manhattan_distance(newNode->state) + newNode->depth;
				q.insert(*newNode);
			}

			newNode->state[ind] = current_n->state[ind];
			newNode->state[ind + 4] = current_n->state[ind + 4];

		}

		//слева направо
		if (ind % 4 > 0)
		{
			Node* newNode = new Node();

			newNode->depth = current_n->depth + 1;
			newNode->pred = current_n;
			newNode->state.resize(16);

			for (int i = 0; i < 16; i++)
				newNode->state[i] = current_n->state[i];

			newNode->state[ind] = current_n->state[ind - 1];
			newNode->state[ind - 1] = current_n->state[ind];

			if (close.count(ToClose({ newNode->state })) == 0)
			{
				newNode->rating = manhattan_distance(newNode->state) + newNode->depth;
				q.insert(*newNode);
			}

			newNode->state[ind] = current_n->state[ind];
			newNode->state[ind - 1] = current_n->state[ind - 1];
		}

		//справа налево
		if (ind % 4 < 3)
		{
			Node* newNode = new Node();

			newNode->depth = current_n->depth + 1;
			newNode->pred = current_n;
			newNode->state.resize(16);

			for (int i = 0; i < 16; i++)
				newNode->state[i] = current_n->state[i];

			newNode->state[ind] = current_n->state[ind + 1];
			newNode->state[ind + 1] = current_n->state[ind];

			if (close.count(ToClose({ newNode->state })) == 0)
			{
				newNode->rating = manhattan_distance(newNode->state) + newNode->depth;
				q.insert(*newNode);
			}

			newNode->state[ind] = current_n->state[ind];
			newNode->state[ind + 1] = current_n->state[ind + 1];
		}
	}
}
