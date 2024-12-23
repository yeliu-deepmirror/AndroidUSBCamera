package com.mobili.usbcamera.utils;

import android.util.Log;
import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import javax.jmdns.JmDNS;
import javax.jmdns.ServiceInfo;

public class MDNSHelper {
    private static final String TAG = "[MOBILI] MDNSHelper";
    private JmDNS jmdns;

    /**
     * Start the mDNS service with the given hostname.
     * @param hostname The desired mDNS hostname.
     * @param serviceName The name of the service to advertise.
     * @param serviceType The type of service (e.g., "_http._tcp.local.").
     * @param port The port for the service.
     * @return True if the service started successfully, false otherwise.
     */
    public boolean begin(String hostname, String serviceName, String serviceType, int port) {
        try {
            // Get the current IP address of the device
            InetAddress ipAddress = getLocalIpAddress();
            if (ipAddress == null) {
                Log.e(TAG, "Failed to get local IP address.");
                return false;
            }

            // Create and start JmDNS instance
            jmdns = JmDNS.create(ipAddress, hostname);

            // Register the service
            ServiceInfo serviceInfo = ServiceInfo.create(serviceType, serviceName, port, "Android mDNS Service");
            jmdns.registerService(serviceInfo);

            Log.i(TAG, "mDNS started successfully with hostname: " + hostname + ", ip: " + ipAddress.toString());
            return true;
        } catch (IOException e) {
            Log.e(TAG, "Error starting mDNS: " + e.getMessage(), e);
            return false;
        }
    }

    /**
     * Stop the mDNS service.
     */
    public void stop() {
        if (jmdns != null) {
            try {
                jmdns.unregisterAllServices();
                jmdns.close();
                jmdns = null;
                Log.i(TAG, "mDNS stopped successfully.");
            } catch (IOException e) {
                Log.e(TAG, "Error stopping mDNS: " + e.getMessage(), e);
            }
        }
    }

    /**
     * Get the local IP address of the device.
     * @return The local IP address or null if not found.
     */
    private InetAddress getLocalIpAddress() {
        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface networkInterface = interfaces.nextElement();
                Enumeration<InetAddress> addresses = networkInterface.getInetAddresses();
                while (addresses.hasMoreElements()) {
                    InetAddress address = addresses.nextElement();
                    if (!address.isLoopbackAddress() && address.isSiteLocalAddress()) {
                        return address;
                    }
                }
            }
        } catch (SocketException e) {
            Log.e(TAG, "Error getting local IP address: " + e.getMessage(), e);
        }
        return null;
    }
}
