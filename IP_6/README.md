# Лабораторная работа №6: Kustomize и Helm

## Структура проекта
<pre>
lab-6/
├── todo-infrastructure/                    # Инфраструктура (БД)
│   ├── mongodb/
│   │   ├── helm/mongodb-chart/            # Helm чарт MongoDB
│   │   │   ├── Chart.yaml
│   │   │   ├── values.yaml
│   │   │   ├── values-dev.yaml
│   │   │   └── templates/
│   │   │       ├── secret.yaml
│   │   │       ├── configmap.yaml
│   │   │       ├── service.yaml
│   │   │       └── statefulset.yaml
│   │   └── kustomize/                     # Kustomize вариант
│   │       ├── base/
│   │       └── overlays/dev/
│   └── README.md                          # Контракт для приложения
│
└── todo-app/                              # Приложение
    ├── backend/                           # Express.js сервер
    │   ├── server.js
    │   ├── package.json
    │   └── Dockerfile
    ├── frontend/                          # React клиент
    │   ├── src/
    │   ├── Dockerfile
    │   └── nginx.conf
    └── k8s/
        └── kustomize/                     # Kustomize манифесты
            ├── base/                      # Базовые манифесты
            │   ├── backend-deployment.yaml
            │   ├── backend-service.yaml
            │   ├── frontend-deployment.yaml
            │   ├── frontend-service.yaml
            │   └── kustomization.yaml
            └── overlays/
                ├── dev/                   # Dev окружение
                │   ├── kustomization.yaml
                │   └── secret.yaml
                └── prod/                  # Prod окружение
                    ├── kustomization.yaml
                    └── secret.yaml
</pre>

## Инструкция по запуску

Проверка установки


docker --version          # Docker Desktop с Kubernetes
kubectl version           # Kubernetes CLI
helm version              # Helm 3.x

### Шаг 1

Переход в папку инфраструктуры:   cd todo-infrastructure

Установка MongoDB через Helm:   helm upgrade --install mongodb ./mongodb/helm/mongodb-chart --namespace todo-dev --create-namespace -f ./mongodb/helm/mongodb-chart/values-dev.yaml

Проверка:   kubectl get pods -n todo-dev

### Шаг 2

Переход в папку приложения:   cd ../todo-app

Применение конфигурации:   kubectl apply -k k8s/kustomize/overlays/dev

Проверка подов:   kubectl get pods -n todo-dev

### Шаг 3

Логи бекенда:   kubectl logs deployment/backend -n todo-dev

Проброс порта для фронтенда:   kubectl port-forward service/frontend 3000:80 -n todo-dev

Открыть в браузере:   http://localhost:3000

Проверить статус всех ресурсов:   kubectl get all -n todo-dev


Если возникает проблема ImagePullBackOff, то нужно прописать docker pull mongo:6.0