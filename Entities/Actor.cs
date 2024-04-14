using SFML.System;
using SFML.Graphics;

namespace Necrowarp.Entities
{
    class Actor : Drawable
    {
        private Vector2u position;
        private Sprite sprite;

        private bool alive;

        public Vector2u Position
        {
            get => position;
            set
            {
                position = value;
                sprite.Position = new Vector2f(value.X, value.Y) * 16.0f;
            }
        }

        public bool Alive
        {
            get => alive;
            set => alive = value;
        }

        public Actor(Vector2u startingPosition, Texture texture)
        {
            sprite = new Sprite(texture);
            Position = startingPosition;
        }

        public void Draw(RenderTarget target, RenderStates states) => target.Draw(sprite, states);
    }
}
