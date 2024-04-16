using SFML.System;

namespace Necrowarp
{
	public class Grid<T>
	{
		private T[,] grid;

		private readonly uint width;
		private readonly uint height;

		public Grid(uint width, uint height)
		{
			grid = new T[width, height];

			this.width = width;
			this.height = height;
		}

		public Grid(in Vector2u size)
		{
			grid = new T[size.X, size.Y];

			width = size.X;
			height = size.Y;
		}

		public T this[uint x, uint y]
		{
			get => grid[x, y];
			set => grid[x, y] = value;
		}

		public T this[in Vector2u position]
		{
			get => grid[position.X, position.Y];
			set => grid[position.X, position.Y] = value;
		}

		public uint Width => width;
		public uint Height => height;

		public T[,] Data => grid;

		public void Set(in T to)
		{
			for (uint y = 0; y < height; y++)
				for (uint x = 0; x < width; x++)
					grid[x, y] = to;
		}
	}

	public class ReadonlyGrid<T>
	{
		private readonly T[,] grid;

		private readonly uint width;
		private readonly uint height;

		/// <summary>
		/// Construct a Readonly Grid from a deep copy of another grid's data.
		/// </summary>
		public ReadonlyGrid(uint width, uint height, in T[,] data)
		{
			grid = new T[width, height];

			this.width = width;
			this.height = height;

			for (uint y = 0; y < height; y++)
				for (uint x = 0; x < width; x++)
					grid[x, y] = data[x, y];
		}

		/// <summary>
		/// Construct a Readonly Grid from a deep copy of another grid's data.
		/// </summary>
		public ReadonlyGrid(in Vector2u size, in T[,] data)
		{
			grid = new T[width, height];

			width = size.X;
			height = size.Y;

			for (uint y = 0; y < height; y++)
				for (uint x = 0; x < width; x++)
					grid[x, y] = data[x, y];
		}

		/// <summary>
		/// Construct a Readonly Grid from a shallow copy of another grid.
		/// </summary>
		public ReadonlyGrid(in Grid<T> other)
		{
			grid = other.Data;

			width = other.Width;
			height = other.Height;
		}

		public T this[uint x, uint y] => grid[x, y];
		public T this[in Vector2u position] => grid[position.X, position.Y];

		public uint Width => width;
		public uint Height => height;

		public T[,] Data => grid;
	}
}
