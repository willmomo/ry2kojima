package jp.rkojima.volumecontrol;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothHeadset;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

import java.util.List;

/**
 * Created by kojima on 2015/09/29.
 */
public class BluetoothReceiver extends BroadcastReceiver {
    private BluetoothHeadset mBluetoothHeadset;
    private Context mContext;
    private BluetoothProfile.ServiceListener mProfileListener = new BluetoothProfile.ServiceListener() {
        @Override
        public void onServiceConnected(int i, BluetoothProfile bluetoothProfile) {
            if (i == BluetoothProfile.HEADSET) {
                String msg = "onServiceConnected: ヘッドセットっぽい";
                mBluetoothHeadset = (BluetoothHeadset)bluetoothProfile;
                List<BluetoothDevice> devices = mBluetoothHeadset.getConnectedDevices();
                if (devices.size() > 0) {
                    Log.d("VCVCVC", msg + " ついたっぽい");
                    Toast.makeText(mContext.getApplicationContext(), "VC: " + msg + " ついたっぽい", Toast.LENGTH_SHORT).show();
                } else {
                    Log.d("VCVCVC", msg + "はずれたっぽい");
                    Toast.makeText(mContext.getApplicationContext(), "VC: " + msg + "はずれたっぽい", Toast.LENGTH_SHORT).show();
                }
            }
        }

        @Override
        public void onServiceDisconnected(int i) {
            if (i == BluetoothProfile.HEADSET) {
                mBluetoothHeadset = null;
            }
        }
    };

    public void onReceive(Context context, Intent intent) {
        mContext = context;
        String action = intent.getAction();

        Log.d("VCVCVC", action);
        Toast.makeText(context.getApplicationContext(), "VC: " + action, Toast.LENGTH_SHORT).show();

        // http://android.roof-balcony.com/bluetooth/bluetooth_headset_getstate/
        // bluetooth headset をリストアップする

        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        bluetoothAdapter.getProfileProxy(context, mProfileListener, BluetoothProfile.HEADSET);
    }
}
