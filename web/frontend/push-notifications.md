# Push notifications

IMPORTANT: push notifications are ONLY shown if the browser (..or the installed app) is running. 

Push notifications (subscription, sending, and receiving) is copperation between the 
- web app, 
- the browser's user agent (aka the browser),
- the browser's push service (aka the push service), and
- the app backend.

The web app SUBSCRIBES to push notifications through the browser's user agent (see MDN Push API).
The backend SENDS (asynchronously) a push notification through the browser's push service (see NPM Web-Push).
The browser's user agent relays the push notification to the web app service worker even if the app is closed.


The app identifies before the push service using pre-generated VAPID keys.
VAPID keys (public and private) are only generated once per application.

The VAPID public key must be known/disclosed in the web client to subscribe to push notifications.
Both VAPID public and private keys must be known in the backend to be able to send push notifications. 

The web client subscribes to push notifications, and receives subcription details (endpoint, key, auth).
The web client sends the subscription details to the backend.
Even if already subscribed, subcription details may have changed since the last web client launch.
The web client must keep the backend up to date with the updated subscription.
Unregistering the web client service worker invalidates the subcription.

The backend provides both the VAPID and subscription details each time a push notification call is made.


### Generating the VAPID keys:
```
npx web-push generate-vapid-keys --json > ./secrets/vapid.json

{
  "publicKey": "BD_uUGFD8xT3nErTmYhUwGjzzGC4Ny63Qr6c-lXL119NGkosV8tUuC_u3lPRcpOaOtqSg0N5gDkbpp0ogmDdXFM",
  "privateKey": "fRcj...XsMQ"
}
```

### Subscribe to push notifications:
```
--- app/index.js ---
  navigator.serviceWorker.ready.then(
    registration => registration.pushManager.getSubscription().then(
      subscription => {
        if (!subscription) {
          const vapidPublicKey = 'BD_uUGFD8xT3nErTmYhUwGjzzGC4Ny63Qr6c-lXL119NGkosV8tUuC_u3lPRcpOaOtqSg0N5gDkbpp0ogmDdXFM';
          registration.pushManager.subscribe({
            userVisibleOnly: true,
            applicationServerKey: urlB64ToUint8Array(vapidPublicKey)
          })
          .then(
            subscription => {
              console.log(subscription);
            }
          )
        }
      }
    )
  );

--- subscription ---
  {
      "endpoint": "https://fcm.googleapis.com/fcm/send/c-xKQKSXGEQ:APA91bGd4zEbr7bZ2YI2V5Um27sZULEztBnBQFp5yAZY5fM1kAO8Emwl8_Oh_J8q87UM51b8UZXNNhTkcZmW4hLPQOFnVkaLhXWF7aEvFiesFyyLF6BeVltIrEO9lg_Ze2TTqmVNl3hY",
      "expirationTime": null,
      "keys": {
          "p256dh": "BF25jliofts8oicmBRwa4-oa2G479jyklsRej6nkHjn0f-IE5UADi5kWaZL2Qxkdhs8iLu-hY-GNuoUagSlnLF8",
          "auth": "-vx3-QQxfUYEeN2DeRK0WQ"
      }
  }
```

### Debug: send a push notification:
```
npx web-push send-notification \
  --endpoint="https://fcm.googleapis.com/fcm/send/c-xKQKSXGEQ:APA91bGd4zEbr7bZ2YI2V5Um27sZULEztBnBQFp5yAZY5fM1kAO8Emwl8_Oh_J8q87UM51b8UZXNNhTkcZmW4hLPQOFnVkaLhXWF7aEvFiesFyyLF6BeVltIrEO9lg_Ze2TTqmVNl3hY" \
  --key="BF25jliofts8oicmBRwa4-oa2G479jyklsRej6nkHjn0f-IE5UADi5kWaZL2Qxkdhs8iLu-hY-GNuoUagSlnLF8" \
  --auth="-vx3-QQxfUYEeN2DeRK0WQ" \
  --payload="Hello world" \
  --vapid-subject="mailto:spam.frontline+vapid@gmail.com" \
  --vapid-pubkey="BD_uUGFD8xT3nErTmYhUwGjzzGC4Ny63Qr6c-lXL119NGkosV8tUuC_u3lPRcpOaOtqSg0N5gDkbpp0ogmDdXFM" \
  --vapid-pvtkey="fRcj...XsMQ"
```

## Frontend

### App
```js
window.addEventListener('load', () => {
  navigator.serviceWorker.register('sw.js');
  getSubscription()
    .then(postSubscriptionToBackend);
});

button.addEventListener('click', () => {
  getSubscription()
    .then(subscription => postNotification(subscription, 'Hello world!'));
});
```

### Client
```js
const vapidPublicKey = 'BD_uUGFD8xT3nErTmYhUwGjzzGC4Ny63Qr6c-lXL119NGkosV8tUuC_u3lPRcpOaOtqSg0N5gDkbpp0ogmDdXFM';

// A new subscription details is generated for EACH time pushManager.subscribe() is called
// A subscription details MAY change even if pushManager.getSubscription() returns a subscription
// A subscription expires once the service worker is unregistered
// Pushing a message over an expired subscription will fail and won't be delivered

export const getSubscription = () => (
  navigator.serviceWorker.ready.then(
    registration => registration.pushManager.getSubscription().then(
      subscription => subscription || 
        registration.pushManager.subscribe({
          userVisibleOnly: true,
          applicationServerKey: urlB64ToUint8Array(vapidPublicKey)
        })
    )
  )
);

const postSubscriptionToBackend = (subscription) => (
  fetch(new URL('/api/subscription', backendUrl), {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(subscription)
  })
);

const postNotification = (subscription, title) => (
  fetch(new URL('/api/notification', backendUrl), {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({ subscription, title })
  })
);
```

### Display
```js
// Browsers (Firefox and Safari) may ignore Notification.requestPermission() 
//  unless in response to a user gesture

const enableNotifications = () => (
  Notification.requestPermission()
);

const displayNotification = (title, options) => (
  Notification.permission !== 'granted' 
    ? Promise.reject(Error('not granted'))
    : navigator.serviceWorker.getRegistration()
      .then(
        registration => registration.showNotification(title, options)
      )
);
```

### Service worker
```js
// Firefox:
//  - won't display actions at all
//  - will fire notificationclose on 'Close', notificationclick otherwise
//  - will fire notificationclose on previously timed out notification with the same tag before showing it again  
//
// Chrome:
//  - will display actions in the Settings
//  - will fire notificationclose on 'Close', notificationclick + action (if any) otherwise
//  - won't display notification with the same tag, if dismissed after timeout

self.addEventListener('notificationclick', (event) => {
  const { notification, action } = event;
  const { data } = notification;
  console.log('handleNotificationClick:', { action, data });
});

self.addEventListener('notificationclose', (event) => {
  const { notification: { data } } = event;
  console.log('handleNotificationClose:', { data });
});

self.addEventListener('push', (event) => {
  const { data } = event;
  console.log('handlePush:', { data });
  event.waitUntil(
    self.registration.showNotification(data.text())
  );
});
```

## Backend

### Send notification api
```js
const vapidKeys = {
  privateKey: process.env.PRIVATE_KEY,
  publicKey: 'BD_uUGFD8xT3nErTmYhUwGjzzGC4Ny63Qr6c-lXL119NGkosV8tUuC_u3lPRcpOaOtqSg0N5gDkbpp0ogmDdXFM'
};

const vapidDetails = {
  ...vapidKeys,
  subject: 'mailto:spam.frontline+vapid@gmail.com'
};

app.post('/api/subscription', (req, res) => {
  const subscription = req.body;
  res.setHeader('Content-Type', 'application/json');
  res.send(JSON.stringify({ data: { success: true } }));
});

app.post('/api/notification', (req, res) => {
  const { subscription, title } = req.body;
  webPush.sendNotification(subscription, title, { vapidDetails })
    .then(({ statusCode, body, headers }) => {
      console.log('webPush.sendNotification: response:', { statusCode, body, headers })
      res.status(statusCode);
      Object.entries(headers).forEach(([name, value]) => res.header(name, value));
      res.send(body);
    });
});
```
