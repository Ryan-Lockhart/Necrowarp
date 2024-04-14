using SFML.System;
using SFML.Window;
using SFML.Graphics;

namespace Necrowarp
{
    enum GamePhase
    {
        Start,
        Play,
        End
    }

    internal class Program
	{
        static Vector2u WindowSize = new Vector2u(1280, 720);

        static RenderWindow window = new RenderWindow(new VideoMode(WindowSize.X, WindowSize.Y), "Necrowarp", Styles.Default);
        static Map map = new Map(WindowSize.X, WindowSize.Y);

        static Font font = new Font("Assets\\undead_pixel.ttf");

        static Sprite energySprite = new Sprite(new Texture("Assets\\energy.png"));

        static GamePhase phase = GamePhase.Start;

        static void Main(string[] args)
		{
            window.SetIcon(16, 16, new Image("Assets\\icon.png").Pixels);

			window.Closed += OnClosed;
			window.KeyPressed += OnKeyPressed;

            map.GameOverEvent += () => phase = GamePhase.End;

			while (window.IsOpen)
			{
                Update();
                Render();
            }

			window.Close();
		}

        static void Update()
        {
            window.DispatchEvents();

            if (phase == GamePhase.Play)
                map.Update();
        }

        static void Render()
        {
            window.Clear(Color.Black);

            switch (phase)
            {
                case GamePhase.Start:
                    Text titleText = new Text("Necrowarp", font, 128)
                    {
                        FillColor = Color.Magenta
                    };
                    var bounds = titleText.GetLocalBounds();
                    titleText.Position = window.GetView().Center - new Vector2f(bounds.Width / 2, bounds.Height);

                    Text startText = new Text("Press Space to begin...", font, 32)
                    {
                        FillColor = Color.White
                    };
                    bounds = startText.GetLocalBounds();
                    startText.Position = window.GetView().Center + new Vector2f(-bounds.Width / 2, bounds.Height * 2);

                    window.Draw(titleText);
                    window.Draw(startText);

                    break;
                case GamePhase.Play:
                    window.Draw(map);

                    for (int i = 0; i < map.Player.Energy; ++i)
                    {
                        energySprite.Position = new Vector2f(i * 16, 0);
                        window.Draw(energySprite);
                    }
                    break;
                case GamePhase.End:
                    Text gameSummaryText = new Text($"You summoned {map.SummonCount} skeletons from beyond the mortal coil.\n\n\tYour skeletons slew {map.KillCount} arrogant adventurers!", font, 16)
                    {
                        FillColor = Color.White
                    };
                    bounds = gameSummaryText.GetLocalBounds();
                    gameSummaryText.Position = window.GetView().Center - new Vector2f(bounds.Width / 2, -bounds.Height * 4);

                    Text gameOverText = new Text("Game Over", font, 64)
                    {
                        FillColor = Color.Red
                    };
                    bounds = gameOverText.GetLocalBounds();
                    gameOverText.Position = window.GetView().Center - new Vector2f(bounds.Width / 2, bounds.Height);

                    Text restartText = new Text("Press R to restart", font, 32)
                    {
                        FillColor = Color.White
                    };
                    bounds = restartText.GetLocalBounds();
                    restartText.Position = window.GetView().Center + new Vector2f(-bounds.Width / 2, bounds.Height * 2);

                    window.Draw(gameSummaryText);
                    window.Draw(gameOverText);
                    window.Draw(restartText);
                    break;
                default:
                    break;
            }

            window.Display();
        }

		static void OnClosed(object? sender, EventArgs e)
		{
            RenderWindow? window = sender as RenderWindow;

            window?.Close();
        }

		static void OnKeyPressed(object? sender, KeyEventArgs e)
		{
			RenderWindow? window = sender as RenderWindow;

            switch (e.Code)
            {
                case Keyboard.Key.Escape:
                    window?.Close();
                    return;
                default:
                    break;
            }

            switch (phase)
            {
                case GamePhase.Start:
                    switch (e.Code)
                    {
                        case Keyboard.Key.Space:
                            phase = GamePhase.Play;
                            return;
                        default:
                            break;
                    }
                    return;
                case GamePhase.Play:
                    switch (e.Code)
                    {
                        case Keyboard.Key.Space:
                            map.TryWarp();
                            return;
                        case Keyboard.Key.Numpad5:
                            map.Pass();
                            return;
                        default:
                            break;
                    }

                    Vector2i movement = new Vector2i(0, 0);

                    if (e.Code == Keyboard.Key.W || e.Code == Keyboard.Key.Up || e.Code == Keyboard.Key.Numpad7 || e.Code == Keyboard.Key.Numpad8 || e.Code == Keyboard.Key.Numpad9)
                        movement.Y--;
                    if (e.Code == Keyboard.Key.S || e.Code == Keyboard.Key.Down || e.Code == Keyboard.Key.Numpad1 || e.Code == Keyboard.Key.Numpad2 || e.Code == Keyboard.Key.Numpad3)
                        movement.Y++;
                    if (e.Code == Keyboard.Key.A || e.Code == Keyboard.Key.Left || e.Code == Keyboard.Key.Numpad1 || e.Code == Keyboard.Key.Numpad4 || e.Code == Keyboard.Key.Numpad7)
                        movement.X--;
                    if (e.Code == Keyboard.Key.D || e.Code == Keyboard.Key.Right || e.Code == Keyboard.Key.Numpad3 || e.Code == Keyboard.Key.Numpad6 || e.Code == Keyboard.Key.Numpad9)
                        movement.X++;

                    if (movement != new Vector2i(0, 0))
                        map.TryMove(movement);
                    return;
                case GamePhase.End:
                    switch (e.Code)
                    {
                        case Keyboard.Key.R:
                            map.Restart();
                            phase = GamePhase.Play;
                            return;
                        default:
                            break;
                    }
                    return;
            }
        }
	}
}
