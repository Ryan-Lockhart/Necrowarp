using SFML.System;
using SFML.Graphics;
using System.IO;

namespace Necrowarp.Entities
{
	public class Skeleton : Actor
	{
		private Adventurer? target = null;

        public Skeleton(Vector2u startingPosition) : base(startingPosition, new Texture("Assets\\skeleton.png"))
        {
            Alive = true;
        }

        public void Update(Map map)
        {
            if (!Alive)
                return;

            if (map.Adventurers.Count <= 0)
            {
                target = null;
                return;
            }

			uint closest = uint.MaxValue;

			foreach (var adventurer in map.Adventurers)
			{
				if (!adventurer.Alive)
					continue;

				uint distance = AStar.Distance(adventurer.Position, Position);

				if (distance < closest)
				{
					target = adventurer;
					closest = distance;
				}
			}

			if (target == null)
				return;

			if (AStar.Distance(target.Position, Position) <= 1)
			{
				map.Clash(target, this);
				return;
			}

			if (AStar.HasLineOfSight(Position, target.Position, map))
			{
				Vector2u newPos = Position;

				if (target.Position.X < Position.X)
					newPos.X--;
				else if (target.Position.X > Position.X)
					newPos.X++;
				if (target.Position.Y < Position.Y)
					newPos.Y--;
				else if (target.Position.Y > Position.Y)
					newPos.Y++;

				if (!map[newPos, Map.EntityType.All])
					Position = newPos;
			}
			else
			{
				var path = AStar.CalculatePath(Position, target.Position, map);

				if (path != null)
					Position = path.Pop();
			}
        }
    }
}
