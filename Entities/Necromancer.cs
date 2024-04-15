using SFML.System;
using SFML.Graphics;

namespace Necrowarp.Entities
{
	public class Necromancer : Actor
    {
        private int energy;
        private bool acted;

        public int Energy
        {
            get => energy;
            set => energy = Math.Clamp(value, 0, 5);
        }

        public bool Acted
        {
            get => acted;
            set => acted = value;
        }

        public bool HasEnergy => energy > 0;

        public Necromancer(Vector2u startingPosition) : base(startingPosition, new Texture("Assets\\player.png"))
        {
            Alive = true;
            Energy = 3;
        }
    }
}
