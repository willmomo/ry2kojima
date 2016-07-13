using System;
using System.Drawing;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;	
namespace fe0r
{
	public class DxForm : Form
	{
		public const int SCREEN_W=640;
		public const int SCREEN_H=480;
		public const bool FULL_SCREEN=false;
		public Device mDev = null; // Our rendering device
		public Sprite mSprite=null;
		public DxForm()
		{
			InitializeComponents();
			CenterToScreen();
		}
		void InitializeComponents() {
			this.SuspendLayout();
			this.Size = new System.Drawing.Size(SCREEN_W, SCREEN_H);
			this.FormBorderStyle =FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.ResumeLayout(false);
		}
		//////////////////////////////////////////////////////
		/// 
		public bool InitDx9()
		{
			try
			{
				PresentParameters presentParams = new PresentParameters();
				presentParams.Windowed=!FULL_SCREEN;
				presentParams.SwapEffect = SwapEffect.Discard;
				mDev = new Device(0, DeviceType.Hardware, this, CreateFlags.SoftwareVertexProcessing, presentParams);
				mSprite =new Sprite(mDev);
				return true;
			}
			catch (DirectXException)
	        { 
	            return false; 
	        }
		
		}
		protected override void OnKeyPress(System.Windows.Forms.KeyPressEventArgs e)
		{
			if ((int)(byte)e.KeyChar == (int)System.Windows.Forms.Keys.Escape)
				this.Close(); // Esc was pressed
		}		
	}
	public class DxUtil
	{

		
		
	}
}
