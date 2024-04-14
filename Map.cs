using SFML.System;
using SFML.Graphics;

using Necrowarp.Entities;

namespace Necrowarp
{
    class Map : Drawable
    {
        public enum EntityType
        {
            Wall,
            Skull,
            All
        }

        private Vector2u size;
        public Vector2u Size => size;
        public uint Area => size.X * size.Y;

        private bool[,] walls;
        private bool[,] skulls;

        private Necromancer player;

        private List<Adventurer> adventurers = new List<Adventurer>();
        private List<Skeleton> skeletons = new List<Skeleton>();

        private bool gameOver = false;

        private int summonCount = 0;
        private int killCount = 0;

        public int SummonCount => summonCount;
        public int KillCount => killCount;

        public bool GameOver
        {
            get => gameOver;
            set
            {
                gameOver = value;
                if (value)
                    GameOverEvent?.Invoke();
            }
        }

        public event Action GameOverEvent = delegate { };

        private RectangleShape wallShape = new RectangleShape(new Vector2f(16, 16))
        {
            FillColor = new Color(128, 128, 128)
        };

        private RectangleShape floorShape = new RectangleShape(new Vector2f(16, 16))
        {
            FillColor = new Color(64, 64, 64)
        };

        private Sprite skullSprite = new Sprite(new Texture("Assets\\skull.png"));

        private void SpawnSkulls(int count)
        {
            for (int i = 0; i < count; ++i)
                this[FindOpen(), EntityType.Skull] = true;
        }

        private void SpawnAdventurers(int count)
        {
            for (int i = 0; i < count; ++i)
                adventurers.Add(new Adventurer(FindOpen(Player.Position, 8)));
        }

        public Map(uint width, uint height)
        {
            size = new Vector2u(width / 16, height / 16);

            walls = new bool[size.X, size.Y];
            skulls = new bool[size.X, size.Y];

            Random random = new Random();

            for (uint y = 0; y < size.Y; ++y)
            {
                for (uint x = 0; x < size.X; ++x)
                {
                    bool isBorder = x == 0 || y == 0 || x == size.X - 1 || y == size.Y - 1;
                    walls[x, y] = isBorder ? true : random.Next() % 4 == 0;
                }
            }

            for (int i = 0; i < 5; ++i)
            {
                for (uint y = 0; y < size.Y; ++y)
                {
                    for (uint x = 0; x < size.X; ++x)
                    {
                        if (x == 0 || y == 0 || x == size.X - 1 || y == size.Y - 1) continue;

                        int neighbours = 0;

                        for (int ny = y > 0 ? -1 : 0; ny <= (y < size.Y - 1 ? 1 : 0); ++ny)
                            for (int nx = x > 0 ? -1 : 0; nx <= (x < size.X - 1 ? 1 : 0); ++nx)
                                neighbours += walls[x + nx, y + ny] ? 1 : 0;

                        if (neighbours > 4)
                            walls[x, y] = true;
                        else if (neighbours < 4)
                            walls[x, y] = false;
                        else continue;
                    }
                }
            }

            player = new Necromancer(FindOpen());

            SpawnAdventurers(3);
            SpawnSkulls(5);
        }

        public void Restart()
        {
            gameOver = false;

            summonCount = 0;
            killCount = 0;

            skeletons.Clear();
            adventurers.Clear();

            Random random = new Random();

            for (uint y = 0; y < size.Y; ++y)
            {
                for (uint x = 0; x < size.X; ++x)
                {
                    bool isBorder = x == 0 || y == 0 || x == size.X - 1 || y == size.Y - 1;
                    walls[x, y] = isBorder ? true : random.Next() % 4 == 0;
                }
            }

            for (int i = 0; i < 5; ++i)
            {
                for (uint y = 0; y < size.Y; ++y)
                {
                    for (uint x = 0; x < size.X; ++x)
                    {
                        if (x == 0 || y == 0 || x == size.X - 1 || y == size.Y - 1) continue;

                        int neighbours = 0;

                        for (int ny = y > 0 ? -1 : 0; ny <= (y < size.Y - 1 ? 1 : 0); ++ny)
                            for (int nx = x > 0 ? -1 : 0; nx <= (x < size.X - 1 ? 1 : 0); ++nx)
                                neighbours += walls[x + nx, y + ny] ? 1 : 0;

                        if (neighbours > 4)
                            walls[x, y] = true;
                        else if (neighbours < 4)
                            walls[x, y] = false;
                        else continue;
                    }
                }
            }

            player = new Necromancer(FindOpen());

            SpawnAdventurers(3);
            SpawnSkulls(5);
        }

        public void Draw(RenderTarget target, RenderStates states)
        {
            for (uint y = 0; y < size.Y; ++y)
            {
                for (uint x = 0; x < size.X; ++x)
                {
                    Vector2f pos = new Vector2f(x, y) * 16.0f;

                    if (walls[x, y])
                    {
                        wallShape.Position = pos;
                        target.Draw(wallShape, states);

                        continue;
                    }
                    else
                    {
                        floorShape.Position = pos;
                        target.Draw(floorShape, states);

                        if (skulls[x, y])
                        {
                            skullSprite.Position = pos;
                            target.Draw(skullSprite, states);
                        }
                    }
                }
            }

            foreach (var skeleton in skeletons)
                target.Draw(skeleton, states);

            foreach (var adventurer in adventurers)
                target.Draw(adventurer, states);

            target.Draw(player, states);
        }

        public Necromancer Player => player;
        public List<Adventurer> Adventurers => adventurers;
        public List<Skeleton> Skeletons => skeletons;

        public bool this[Vector2u pos, EntityType type = EntityType.Wall]
        {
            get => type switch
            {
                EntityType.Wall => walls[pos.X, pos.Y],
                EntityType.Skull => skulls[pos.X, pos.Y],
                EntityType.All => walls[pos.X, pos.Y] || skulls[pos.X, pos.Y],
                _ => throw new Exception("Invalid entity type!")
            };

            set
            {
                switch (type)
                {
                    case EntityType.Wall:
                        walls[pos.X, pos.Y] = value;
                        return;
                    case EntityType.Skull:
                        skulls[pos.X, pos.Y] = value;
                        return;
                    case EntityType.All:
                    default:
                        throw new Exception("Invalid entity type!");
                }
            }
        }

        public bool this[uint x, uint y, EntityType type = EntityType.Wall]
        {
            get => type switch
            {
                EntityType.Wall => walls[x, y],
                EntityType.Skull => skulls[x, y],
                EntityType.All => walls[x, y] || skulls[x, y],
                _ => throw new Exception("Invalid entity type!")
            };

            set
            {
                switch (type)
                {
                    case EntityType.Wall:
                        walls[x, y] = value;
                        return;
                    case EntityType.Skull:
                        skulls[x, y] = value;
                        return;
                    case EntityType.All:
                    default:
                        throw new Exception("Invalid entity type!");
                }
            }
        }

        public static uint Distance(Vector2u a, Vector2u b) => (uint)(Math.Max(Math.Abs(a.X - b.X), Math.Abs(a.Y - b.Y)));

        private Vector2u RandomPosition() => new Vector2u((uint)(Random.Shared.Next() % size.X), (uint)(Random.Shared.Next() % size.Y));
        private Vector2u RandomPosition(Random generator) => new Vector2u((uint)(generator.Next() % size.X), (uint)(generator.Next() % size.Y));

		public Vector2u FindOpen()
        {
            Random random = new Random();
            int tries = 0;

            while (tries < Area)
            {
                Vector2u randomPos = RandomPosition(random);

                if (!this[randomPos, EntityType.All])
                    return randomPos;

                ++tries;
            }

            throw new Exception("No open space found!");
        }

        public Vector2u FindOpen(Vector2u position, int distance)
        {
            Random random = new Random();
            int tries = 0;

            while (tries < Area)
            {
                Vector2u randomPos = RandomPosition(random);

                if (Distance(randomPos, position) < distance)
                    continue;

                if (!this[randomPos, EntityType.All])
                    return randomPos;

                ++tries;
            }

            throw new Exception("No open space found!");
        }

        public void TryMove(Vector2i movement)
        {
            Vector2u newPos = Player.Position + (Vector2u)movement;

            if (this[newPos, EntityType.Skull])
            {
                this[newPos, EntityType.Skull] = false;
                skeletons.Add(new Skeleton(newPos));
                ++summonCount;

                ++Player.Energy;
                Player.Position = FindOpen();

                Player.Acted = true;
            }
            else if (!this[newPos, EntityType.Wall])
            {
                Player.Position = newPos;
                Player.Acted = true;
            }
        }

        public void TryWarp()
        {
            if (Player.HasEnergy)
            {
                --Player.Energy;
                Player.Position = FindOpen(Player.Position, 8);
                Player.Acted = true;
            }
        }

		public void Pass() => Player.Acted = true;

        public void Clash(Adventurer adventurer, Skeleton skeleton)
        {
            this[adventurer.Position, EntityType.Skull] = true;
            adventurer.Alive = false;
            ++killCount;

            skeleton.Alive = false;
        }

		public void Update()
        {
            if (!Player.Acted)
                return;


            foreach (var skeleton in Skeletons)
                skeleton.Update(this);

            foreach (var adventurer in Adventurers)
                adventurer.Update(this);

            if (Adventurers.Count <= 0)
                SpawnAdventurers(3);

            Adventurers.RemoveAll(adventurer => !adventurer.Alive);
            Skeletons.RemoveAll(skeleton => !skeleton.Alive);

            Player.Acted = false;
        }
    }
}
