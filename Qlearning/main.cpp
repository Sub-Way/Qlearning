/*
Do what you want license by Prof. J.-M. Hong
*/

#include <iostream>

#define MAX2(a, b)                     ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c)                  (MAX2(MAX2(a, b), (c)))
#define MAX4(a, b, c, d)               (MAX2(MAX3(a, b, c), (d)))

class CellData
{
public:
	double q_[4] = { 0.0, 0.0, 0.0, 0.0 }; // up, down, left, right
	double reward_ = 0.0;

	CellData()
	{}

	double getMaxQ()
	{
		return MAX4(q_[0], q_[1], q_[2], q_[3]);
	}
};

class GridWorld
{
public:
	int i_res_, j_res_;
	CellData *q_arr2d_ = nullptr;

	GridWorld(const int& i_res, const int& j_res)
		: i_res_(i_res), j_res_(j_res)
	{
		q_arr2d_ = new CellData[i_res*j_res];
	}

	CellData& getCell(const int& i, const int& j)
	{
		return q_arr2d_[i + i_res_ * j];
	}

	bool isInside(const int& i, const int& j)
	{
		if (i < 0) return false;
		if (i >= i_res_) return false;

		if (j < 0) return false;
		if (j >= j_res_) return false;

		return true;
	}

	void printSigned(const float& v)
	{
		if (v > 0.0f) printf("+%1.1f", v);
		else if (v < 0.0f) printf("%1.1f", v);
		else
			printf(" 0.0");
	}

	void print()
	{
		for (int j = j_res_ - 1; j >= 0; j--)
		{
			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);

				printf("   "); printSigned(cell.q_[0]); printf("   "); // up
				printf("   ");
			}

			printf("\n");

			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);

				printSigned(cell.q_[2]); printf("   "); printSigned(cell.q_[3]); // left, right
				printf("  ");
			}

			printf("\n");

			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);

				printf("   "); printSigned(cell.q_[1]); printf("   "); // down
				printf("   ");
			}

			printf("\n\n");
		}
	}
};

class Agent
{
public:
	int i_, j_;         // status (s_t) of this agent = position in grid world
	double reward_;      // reward sum

	Agent()
		: i_(0), j_(0), reward_(0.0)
	{}
};

int main()
{
	std::cout << "Hello, grid world!" << std::endl;

	const int world_res_i = 3, world_res_j = 2;

	GridWorld world(world_res_i, world_res_j);

	//for (int j = 0; j < world_res_j; j++)
	//for (int i = 0; i < world_res_i; i++)
	//{
	//   world.getCell(i, j).reward_ = -0.1;
	//}

	world.getCell(2, 1).reward_ = 1.0;
	world.getCell(2, 0).reward_ = -1.0;

	Agent my_agent;

	world.print();

	std::cout << "----------------------------------------" << std::endl;

	for (int t = 0; t < 1000000; t++)
	{
		const int action = rand() % 4; // random policy

		int i = my_agent.i_, j = my_agent.j_;
		int i_old = i, j_old = j;

		switch (action)
		{
		case 0:
			j++; // up
			break;
		case 1:
			j--; // down
			break;
		case 2:
			i--; // left
			break;
		case 3:
			i++; // right
			break;
		}

		if (world.isInside(i, j) == true) // move robot if available
		{
			// move agent
			if (world.getCell(i, j).reward_) {
				world.getCell(i_old, j_old).q_[action] = world.getCell(i, j).reward_;
				my_agent.i_ = i_old;
				my_agent.j_ = j_old;
			}
			else
			{
				world.getCell(i_old, j_old).q_[action] = MAX4(world.getCell(i, j).q_[0], world.getCell(i, j).q_[1], world.getCell(i, j).q_[2], world.getCell(i, j).q_[3]) - 0.1;
				my_agent.i_ = i;
				my_agent.j_ = j;
			}
		}
		else
		{
			// you may give negative reward (penalty) to agent.
			my_agent.i_ = i_old;
			my_agent.j_ = j_old;
		}

		/*std::cout << "Agent status " << my_agent.i_ << " " << my_agent.j_ << " " << my_agent.reward_ << std::endl;
		std::cout << "action " << action << std::endl;

		world.print();*/
	}

	world.print();

	return 0;
}