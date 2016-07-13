		private void Rotate(out double newX, out double newY, double x, double y, double xc, double yc, double theta) {
			y = -y; yc = -yc; // 数学座標と同じ様にするためにy座標値を反転
			newX = (x - xc) * Math.Cos(theta) - (y - yc) * Math.Sin(theta) + xc;
			newY = -1.0 * ( (x - xc) * Math.Sin(theta) + (y - yc) * Math.Cos(theta) + yc );
		}
