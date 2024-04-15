using SFML.System;
using SFML.Graphics;
using System.Numerics;

namespace Necrowarp
{
	public class AStar
	{
		public static uint Distance(in Vector2u start, in Vector2u end) => (uint)Math.Max(Math.Abs((int)end.X - start.X), Math.Abs((int)end.Y - start.Y));

		public static Stack<Vector2u>? CalculatePath(in Vector2u origin, in Vector2u destination, in Map map)
		{
			PriorityQueue<Vector2u, uint> frontier = new PriorityQueue<Vector2u, uint>();
			HashSet<Vector2u> visited = new HashSet<Vector2u>();

			uint initial_distance = Distance(origin, destination);

			frontier.Enqueue(origin, initial_distance);
			visited.Add(origin);

			Dictionary<Vector2u, Vector2u> came_from = new Dictionary<Vector2u, Vector2u>();
			Dictionary<Vector2u, uint> cost_so_far = new Dictionary<Vector2u, uint>();

			came_from[origin] = origin;
			cost_so_far[origin] = initial_distance;

			while (frontier.Count > 0)
			{
				Vector2u currentPosition = frontier.Dequeue();

				if (currentPosition == destination)
					goto PathFound;

				for (int y = -1; y <= 1; ++y)
				{
					for (int x = -1; x <= 1; ++x)
					{
						if (x == 0 && y == 0)
							continue;

						Vector2u currentNeighbour = new Vector2u((uint)(currentPosition.X + x), (uint)(currentPosition.Y + y));

						if (currentNeighbour == destination)
						{
							came_from[currentNeighbour] = currentPosition;
							goto PathFound;
						}

						if (map[currentNeighbour, Map.EntityType.All])
							continue;

						if (visited.Contains(currentNeighbour))
							continue;

						visited.Add(currentNeighbour);

						uint new_cost = Distance(currentNeighbour, destination);

						if (new_cost > cost_so_far[currentPosition])
							continue;

						frontier.Enqueue(currentNeighbour, new_cost);
						came_from[currentNeighbour] = currentPosition;
						cost_so_far[currentNeighbour] = new_cost;
					}
				}
			}

			return null;

		PathFound:

			Stack<Vector2u> path = new Stack<Vector2u>();

			Vector2u current = destination;

			while (current != origin)
			{
				path.Push(current);
				current = came_from[current];
			}

			return path;
		}

		public static bool HasLineOfSight(in Vector2u origin, in Vector2u destination, in Map map)
		{
			if (map[origin, Map.EntityType.Wall] || map[destination, Map.EntityType.Wall])
				return false;

			Vector2f position = (Vector2f)origin;
			Vector2f delta = new Vector2f(Math.Abs((int)destination.Y - origin.Y), Math.Abs((int)destination.Y - origin.Y));

			float step = delta.X >= delta.Y ? delta.X : delta.Y;

			delta /= step;

			for (int i = 0; i <= step;)
			{
				if (map[(uint)position.X, (uint)position.Y, Map.EntityType.Wall])
					return false;

				position += delta;
				++i;
			}

			return true;
		}
	}
}
