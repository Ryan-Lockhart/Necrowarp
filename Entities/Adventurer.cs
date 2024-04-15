using SFML.System;
using SFML.Graphics;

namespace Necrowarp.Entities
{
	public class Adventurer : Actor
    {
        public Adventurer(Vector2u startingPosition) : base(startingPosition, new Texture("Assets\\adventurer.png"))
        {
            Alive = true;
        }

        public void Update(Map map)
        {
            if (!Alive)
                return;

            if (AStar.Distance(map.Player.Position, Position) <= 1)
            {
                map.GameOver = true;
                return;
			}

			if (AStar.HasLineOfSight(Position, map.Player.Position, map))
			{
				Vector2u newPos = Position;

				if (map.Player.Position.X < Position.X)
					newPos.X--;
				else if (map.Player.Position.X > Position.X)
					newPos.X++;
				if (map.Player.Position.Y < Position.Y)
					newPos.Y--;
				else if (map.Player.Position.Y > Position.Y)
					newPos.Y++;

				if (!map[newPos, Map.EntityType.All])
				{
					Position = newPos;
					return;
				}
			}

			var path = AStar.CalculatePath(Position, map.Player.Position, map);

			if (path != null)
				Position = path.Pop();
		}
    }
}
