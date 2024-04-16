using SFML.System;
using SFML.Graphics;

using Necrowarp.Entities;
using System.Numerics;

namespace Necrowarp
{
    public class Map : Drawable
    {
        public enum EntityType
        {
            Wall,
            Skull,
            Occupant,
            All
        }

        private Vector2u size;
        public Vector2u Size => size;
        public uint Area => size.X * size.Y;

        private Grid<bool> walls;
        private Grid<bool> skulls;

        private Grid<uint> safeDistances;

		private Grid<Actor?> occupants;

		private Necromancer player;

        private List<Adventurer> adventurers = new List<Adventurer>();
        private List<Skeleton> skeletons = new List<Skeleton>();

        private bool gameOver = false;

        private int summonCount = 0;
		private int necromancerKillCount = 0;
		private int skeletonKillCount = 0;

        public int SummonCount => summonCount;
		public int NecromancerKillCount => necromancerKillCount;
		public int SkeletonKillCount => skeletonKillCount;

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
            {
                var adventurer = new Adventurer(FindOpen(Player.Position, 8));

				adventurers.Add(adventurer);
                Occupy(adventurer, adventurer.Position);
            }
		}

		private void CalculateSafeDistances()
        {
            for (uint y = 0; y < size.Y; ++y)
            {
				for (uint x = 0; x < size.X; ++x)
                {
                    if (walls[x, y])
                        continue;
                    else
                    {
                        foreach (var adventurer in adventurers)
							safeDistances[x, y] = Math.Min(safeDistances[x, y], AStar.Distance(new Vector2u(x, y), adventurer.Position));
					}
				}
			}
        }

		private bool IsSafe(in Vector2u position, uint minimumDistance) => safeDistances[position.X, position.Y] >= minimumDistance;

		public Map(uint width, uint height)
        {
            if (width % 16 != 0 || height % 16 != 0)
                throw new Exception("Map dimensions must be divisible by 16!");

            size = new Vector2u(width / 16, height / 16);

            walls = new Grid<bool>(size);
            skulls = new Grid<bool>(size);

            safeDistances = new Grid<uint>(size);

            occupants = new Grid<Actor?>(size);

            Random random = new Random();

            for (uint y = 0; y < size.Y; ++y)
            {
                for (uint x = 0; x < size.X; ++x)
                {
                    bool isBorder = x == 0 || y == 0 || x == size.X - 1 || y == size.Y - 1;
                    walls[x, y] = isBorder ? true : random.Next() % 4 == 0;
                    safeDistances[x, y] = uint.MaxValue;
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

                        for (int dy = -1; dy <= 1; ++dy)
                        {
							for (int dx = -1; dx <= 1; ++dx)
							{
                                if (dx == 0 && dy == 0) continue;

                                if (walls[(uint)(x + dx), (uint)(y + dy)])
									++neighbours;
							}
						}

                        if (neighbours > 4)
                            walls[x, y] = true;
                        else if (neighbours < 4)
                            walls[x, y] = false;
                        else continue;
                    }
                }
            }

            player = new Necromancer(FindOpen());

            SpawnAdventurers(5);
            SpawnSkulls(10);
        }

        public void Restart()
        {
            gameOver = false;

            summonCount = 0;
            necromancerKillCount = 0;
            skeletonKillCount = 0;

            skeletons.Clear();
            adventurers.Clear();

            Random random = new Random();

            for (uint y = 0; y < size.Y; ++y)
            {
                for (uint x = 0; x < size.X; ++x)
                {
                    bool isBorder = x == 0 || y == 0 || x == size.X - 1 || y == size.Y - 1;
                    walls[x, y] = isBorder ? true : random.Next() % 4 == 0;
                    occupants[x, y] = null;
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

						for (int dy = -1; dy <= 1; ++dy)
						{
							for (int dx = -1; dx <= 1; ++dx)
							{
								if (dx == 0 && dy == 0) continue;

								if (walls[(uint)(x + dx), (uint)(y + dy)])
									++neighbours;
							}
						}

						if (neighbours > 4)
                            walls[x, y] = true;
                        else if (neighbours < 4)
                            walls[x, y] = false;
                        else continue;
                    }
                }
            }

            player = new Necromancer(FindOpen());

            SpawnAdventurers(5);
            SpawnSkulls(10);
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
				EntityType.Occupant => occupants[pos.X, pos.Y] != null,
				EntityType.All => walls[pos.X, pos.Y] || skulls[pos.X, pos.Y] || occupants[pos.X, pos.Y] != null,
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
                EntityType.Occupant => occupants[x, y] != null,
                EntityType.All => walls[x, y] || skulls[x, y] || occupants[x, y] != null,
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

        public Vector2u FindOpen(Vector2u position, uint distance, bool safe = false)
        {
            Random random = new Random();
            int tries = 0;

            while (tries < Area)
            {
                Vector2u randomPos = RandomPosition(random);
                uint randomDistance = AStar.Distance(randomPos, position);

				if (randomDistance < distance)
                    continue;

                if (safe && !IsSafe(randomPos, distance))
					continue;

                if (!this[randomPos, EntityType.All])
                    return randomPos;

                ++tries;
            }

            throw new Exception("No open space found!");
        }

        private void Occupy(Actor actor, in Vector2u position)
        {
            if (occupants[position] != null)
                throw new Exception("Position already occupied!");

            (occupants[position], occupants[actor.Position]) = (actor, null);
            actor.Position = position;
		}

        public void Move(Actor actor, in Vector2i movement)
        {
            Vector2u newPos = new Vector2u((uint)(actor.Position.X + movement.X), (uint)(actor.Position.Y + movement.Y));

			if (actor is Necromancer necromancer)
			{
				if (this[newPos, EntityType.Skull])
				{
					this[newPos, EntityType.Skull] = false;

					skeletons.Add(new Skeleton(newPos));

					++summonCount;

					try { Occupy(necromancer, FindOpen(necromancer.Position, 8, true)); }
					catch
					{
						Occupy(necromancer, FindOpen(necromancer.Position, 8));
						++necromancer.Energy;
					}

					++Player.Energy;
					Player.Acted = true;
					return;
				}
                else if (this[newPos, EntityType.Occupant] && necromancer.BoneArmor && occupants[newPos] is Adventurer adventurer)
                {
                    Clash(adventurer, necromancer);
					Player.Acted = true;
                }

				if (!this[newPos, EntityType.All])
				{
                    Occupy(necromancer, newPos);
					necromancer.Acted = true;
                    return;
				}

                return;
			}

			if (!this[newPos, EntityType.All])
				Occupy(actor, newPos);
		}


		public void Move(Actor actor, in Vector2u position)
		{
			if (!this[position, EntityType.All])
				Occupy(actor, position);
		}

		public void Warp()
        {
            if (Player.HasEnergy)
            {
				try { Occupy(Player, FindOpen(Player.Position, 8, true)); }
				catch
				{
					Occupy(Player, FindOpen(Player.Position, 8));
					player.Acted = true;
                    return;
				}

				--Player.Energy;
				Player.Acted = true;
            }
		}

		public void Warp(in Vector2u position)
		{
			if (Player.Energy >= 2)
			{
                bool discount = false;

                if (this[position, EntityType.Occupant])
                {
                    if (occupants[position] is Skeleton skeleton)
                    {
						discount = Player.BoneArmor;

						Skeletons.Remove(skeleton);
                        occupants[position] = null;
                        Player.ArmorDurability = 3;
                    }
                    else return;
				}

				Occupy(Player, position);
				Player.Energy -= discount ? 1 : 2;
				player.Acted = true;
			}
		}

		public void Pass() => Player.Acted = true;

        public void Clash(Adventurer adventurer, Skeleton skeleton)
        {
            if (!adventurer.Alive || !skeleton.Alive)
                return;

            adventurer.Alive = false;
			skeleton.Alive = false;

			++skeletonKillCount;
		}

        public void Clash(Adventurer adventurer, Necromancer necromancer)
        {
            if (!adventurer.Alive || !necromancer.Alive)
				return;

            if (necromancer.BoneArmor)
			{
				this[adventurer.Position, EntityType.Skull] = true;

				adventurer.Alive = false;

				--necromancer.ArmorDurability;
				++necromancerKillCount;
			}
			else GameOver = true;
		}

		public void Update()
        {
            if (!Player.Acted)
                return;

            foreach (var skeleton in Skeletons)
                skeleton.Update(this);

            foreach (var adventurer in Adventurers)
                adventurer.Update(this);

            CalculateSafeDistances();

            if (Adventurers.Count <= 0)
				SpawnAdventurers(5 + SkeletonKillCount / 2);

            for (int i = Adventurers.Count - 1; i >= 0; --i)
            {
                var adventurer = Adventurers[i];

                if (adventurer.Alive)
                    continue;

                this[adventurer.Position, EntityType.Skull] = true;
                occupants[adventurer.Position] = null;

                Adventurers.RemoveAt(i);
			}

			for (int i = Skeletons.Count - 1; i >= 0; --i)
			{
				var skeleton = Skeletons[i];

				if (skeleton.Alive)
					continue;

				occupants[skeleton.Position] = null;

				Skeletons.RemoveAt(i);
			}

			Player.Acted = false;
        }
    }
}
