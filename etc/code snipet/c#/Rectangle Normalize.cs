		private Rectangle Normalize(Rectangle org) {
			Rectangle result = org;
			if (org.Width < 0) {
				result.X = org.Right;
				result.Width = -org.Width;
			}
			if (org.Height < 0) {
				result.Y = org.Bottom;
				result.Height = -org.Height;
			}
			return result;
		}
