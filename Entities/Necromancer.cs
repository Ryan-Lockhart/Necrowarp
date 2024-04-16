using SFML.System;
using SFML.Graphics;

namespace Necrowarp.Entities
{
	public class Necromancer : Actor
    {
        private int energy;
        private bool acted;

        private bool boneArmor;
        private int armorDurability;

        private Texture defaultTexture;
        private Texture boneArmorTexture;

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

        public bool BoneArmor
        {
			get => boneArmor;
            set
            {
                if (boneArmor == value)
					return;

                boneArmor = value;

                Sprite.Texture = boneArmor ? boneArmorTexture : defaultTexture;
            }
		}

        public int ArmorDurability
		{
			get => armorDurability;
			set
			{
                if (armorDurability == value) return;

				armorDurability = Math.Clamp(value, 0, 3);

				BoneArmor = armorDurability > 0;
			}
		}

		public bool HasEnergy => energy > 0;

        public Necromancer(Vector2u startingPosition) : base(startingPosition, "Assets\\player.png")
        {
            defaultTexture = Sprite.Texture;
            boneArmorTexture = new Texture("Assets\\player_bone.png");

            Energy = 3;
            Alive = true;

            BoneArmor = false;
            ArmorDurability = 0;
        }
    }
}
