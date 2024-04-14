using SFML.System;
using SFML.Graphics;

namespace Necrowarp.Entities
{
    class Skeleton : Actor
    {
        Adventurer? target = null;

        public Skeleton(Vector2u startingPosition) : base(startingPosition, new Texture("Assets\\skeleton.png"))
        {
            Alive = true;
        }

        public void Update(Map map)
        {
            if (!Alive)
                return;

            // Escape if no adventurers left
            if (map.Adventurers.Count <= 0)
            {
                target = null;
                return;
            }

            // Find closest adventurer
            if (target == null)
            {
                uint closest = uint.MaxValue;

                foreach (var adventurer in map.Adventurers)
                {
                    if (!adventurer.Alive)
                        continue;

                    uint distance = Map.Distance(adventurer.Position, Position);

                    if (distance < closest)
                    {
                        target = adventurer;
                        closest = distance;
                    }
                }
            }

            // Move towards target with Chebyshev distance
            if (target != null)
            {
                if (Map.Distance(target.Position, Position) <= 1)
                {
                    map.Clash(target, this);
                    return;
                }

                Vector2u newPos = Position;

                if (target.Position.X < Position.X)
                    newPos.X--;
                else if (target.Position.X > Position.X)
                    newPos.X++;

                if (target.Position.Y < Position.Y)
                    newPos.Y--;
                else if (target.Position.Y > Position.Y)
                    newPos.Y++;

                if (!map[newPos, Map.EntityType.Wall])
                    Position = newPos;
            }
        }
    }
}
