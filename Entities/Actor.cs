using SFML.System;
using SFML.Graphics;

namespace Necrowarp.Entities
{
    public class PositionEventArgs : EventArgs
    {
		private Vector2u currentPosition;
        private Vector2u lastPosition;

		public Vector2u CurrentPosition => currentPosition;
        public Vector2u LastPosition => lastPosition;

		public PositionEventArgs(Vector2u current, Vector2u last)
		{
			currentPosition = current;
			lastPosition = last;
		}
	}

    public class Actor : Drawable
    {
        private Vector2u position;
        private Sprite sprite;

        private bool alive;

        public event EventHandler<PositionEventArgs> PositionChangedEvent = delegate { };

        public Vector2u Position
        {
            get => position;
            set
            {
                if (position == value)
					return;

				PositionChangedEvent?.Invoke(this, new PositionEventArgs(value, position));

				position = value;
                sprite.Position = new Vector2f(value.X, value.Y) * 16.0f;
            }
        }

        public event Action DeathEvent = delegate { };

        public bool Alive
        {
            get => alive;
            set
            {
				if (alive == value)
					return;

				alive = value;

				if (!alive)
					DeathEvent?.Invoke();
			}
        }

        public Sprite Sprite => sprite;

        public Actor(Vector2u startingPosition, string spritePath)
        {
            sprite = new Sprite(new Texture(spritePath));
            Position = startingPosition;
        }

        public void Draw(RenderTarget target, RenderStates states) => target.Draw(sprite, states);
    }
}
