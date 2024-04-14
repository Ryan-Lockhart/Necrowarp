using SFML.System;
using SFML.Graphics;

namespace Necrowarp.Entities
{
    class Adventurer : Actor
    {
        public Adventurer(Vector2u startingPosition) : base(startingPosition, new Texture("Assets\\adventurer.png"))
        {
            Alive = true;
        }

        public void Update(Map map)
        {
            if (!Alive)
                return;

            if (Map.Distance(map.Player.Position, Position) <= 1)
            {
                map.GameOver = true;
                return;
            }

            Vector2u newPos = Position;

            if (map.Player.Position.X < Position.X)
                newPos.X--;
            else if (map.Player.Position.X > Position.X)
                newPos.X++;
            if (map.Player.Position.Y < Position.Y)
                newPos.Y--;
            else if (map.Player.Position.Y > Position.Y)
                newPos.Y++;

            if (!map[newPos, Map.EntityType.Wall])
                Position = newPos;
        }
    }
}
