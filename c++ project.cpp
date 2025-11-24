#include <bits/stdc++.h>
using namespace std;

struct Phase {
    string title;
    vector<string> steps;
};

struct Roadmap {
    string displayName;
    vector<Phase> phases;
};

// global map: normalized role -> roadmap
unordered_map<string, Roadmap> roadmaps;

// helpers
string normalize(const string &s) {
    string out;
    for (char c : s) {
        if (!isspace((unsigned char)c)) out.push_back(tolower((unsigned char)c));
        else out.push_back(' ');
    }
    // trim
    while (!out.empty() && isspace((unsigned char)out.front())) out.erase(out.begin());
    while (!out.empty() && isspace((unsigned char)out.back())) out.pop_back();
    return out;
}

string keyify(const string &s) {
    string k;
    for (char c : s) {
        if (!isspace((unsigned char)c)) k.push_back(tolower((unsigned char)c));
    }
    return k;
}

void printDivider() {
    cout << "\n" << string(72, '=') << "\n\n";
}

void printRoadmap(const Roadmap &r) {
    printDivider();
    cout << "ROADMAP — " << r.displayName << "\n\n";
    int pidx = 1;
    for (auto &p : r.phases) {
        cout << "PHASE " << pidx++ << " — " << p.title << "\n";
        cout << string(40, '-') << "\n";
        int sidx = 1;
        for (auto &s : p.steps) {
            cout << sidx++ << ". " << s << "\n";
        }
        cout << "\n";
    }
    printDivider();
}

// find best matching roles (exact or substring)
vector<string> findMatches(const string &input) {
    string in = keyify(input);
    vector<string> exacts;
    vector<string> substrs;
    for (auto &kv : roadmaps) {
        string k = kv.first; // already keyified
        if (k == in) exacts.push_back(kv.first);
        else if (k.find(in) != string::npos) substrs.push_back(kv.first);
    }
    if (!exacts.empty()) return exacts;
    if (!substrs.empty()) return substrs;

    // fallback: find roles whose displayName contains input (case-insensitive)
    vector<string> fallback;
    string lowered = normalize(input);
    for (auto &kv : roadmaps) {
        string dn = normalize(kv.second.displayName);
        if (dn.find(lowered) != string::npos) fallback.push_back(kv.first);
    }
    return fallback;
}

void initRoadmaps() {
    // For each role we create a Roadmap with several phases and many detailed steps.
    // Keys are 'keyified' (all letters lower and no spaces) to make matching robust.
    auto add = [&](const string &name, const vector<Phase> &phs) {
        string key = keyify(name);
        roadmaps[key] = Roadmap{name, phs};
    };

    // FRONTEND
    add("Frontend", {
        {"FOUNDATIONS: HTML, CSS, JAVASCRIPT",
            {
                "Learn semantic HTML: elements, forms, accessibility (ARIA).",
                "Deep CSS: box model, positioning, flexbox, grid, BEM methodology.",
                "Responsive design: media queries, mobile-first approach, fluid layouts.",
                "Advanced JavaScript fundamentals: scopes, closures, prototypes, async/await, event loop.",
                "Browser devtools masterclass: network, performance, accessibility auditing."
            }
        },
        {"BUILD TOOLING & WORKFLOW",
            {
                "Package managers: npm, yarn, pnpm — understand lockfiles and semver.",
                "Bundlers and compilers: webpack, Vite, esbuild — config and optimization.",
                "Transpilation and language tooling: Babel, TypeScript (strict mode) and tsconfig.",
                "Linting/Formatting: ESLint with shared configs, Prettier, commit hooks (husky).",
                "Module systems: ESM vs CommonJS; tree-shaking & code-splitting."
            }
        },
        {"FRAMEWORKS & PATTERNS",
            {
                "Pick a major framework: React (hooks & concurrent), Vue (composition API), or Angular — learn core concepts deeply.",
                "State management patterns: local state, lifting state, Context/Redux/MobX/Pinia, Zustand.",
                "Routing, code splitting, SSR vs CSR vs SSG: Next.js/Nuxt/Remix/Gatsby fundamentals.",
                "Component design: composition over inheritance, presentational vs container, accessibility-first components.",
                "Testing: unit (Jest), integration (React Testing Library), E2E (Playwright/Cypress)."
            }
        },
        {"PERFORMANCE, ARCHITECTURE & DEPLOYMENT",
            {
                "Optimize time-to-first-byte and Largest Contentful Paint: critical CSS, preloading, HTTP caching.",
                "Progressive Web Apps (PWA): service workers, offline-first strategies, manifests.",
                "CI/CD for frontend: pipeline basics, preview deployments, automated tests.",
                "Observability: RUM, performance budgets, lighthouse automation and monitoring.",
                "Security basics: CSP, XSS prevention, secure headers, dependency scanning."
            }
        },
        {"CAREER & SOFT SKILLS",
            {
                "Build 4+ portfolio projects demonstrating a full flow (auth, API integration, deployment).",
                "Open-source contributions: small bug fixes -> feature PRs on libraries you use.",
                "System design for frontend: component libraries, design tokens, cross-team contracts.",
                "Mentoring and code reviews: give and receive feedback, write clear PR descriptions.",
                "Keep learning: read RFCs, browser release notes, and explore WebAssembly basics."
            }
        }
    });

    // BACKEND
    add("Backend", {
        {"LANGUAGES & FUNDAMENTALS",
            {
                "Master at least one backend language deeply: Node.js (JS/TS), Python, Java, Go, C# or C++.",
                "Understand networking basics: TCP/IP, HTTP/1.1 vs HTTP/2, TLS fundamentals.",
                "Advanced language features: concurrency models (threads, async, goroutines), memory model, GC behavior.",
                "Data structures & algorithms applied to backend: queues, maps, heaps, graphs for real-world tasks.",
                "Design patterns: factory, repository, strategy, dependency injection."
            }
        },
        {"API DESIGN & COMMUNICATION",
            {
                "RESTful design: resources, status codes, idempotency, pagination, filtering.",
                "Contract-first design: OpenAPI/Swagger, schema evolution & versioning strategies.",
                "gRPC & protobufs: use-cases, streaming, performance trade-offs.",
                "Error handling & observability: structured logs, correlation IDs, distributed tracing.",
                "Authentication/authorization: OAuth2, JWT, sessions, RBAC and secure token storage."
            }
        },
        {"DATA & PERSISTENCE",
            {
                "Relational DBs: schema design, normalization, indexing, transactions, isolation levels.",
                "NoSQL: when to use document stores, wide-column, key-value; pros/cons of eventual consistency.",
                "Caching: in-memory caches (Redis), cache invalidation patterns, TTL, cache-aside.",
                "Migrations & schema evolution tools: Liquibase, Flyway, Alembic.",
                "ORMs vs query builders vs raw SQL: know the tradeoffs, optimize slow queries."
            }
        },
        {"SCALING & OPERATIONS",
            {
                "Horizontal scaling strategies: stateless services, sticky sessions, load balancers.",
                "Message-driven architecture: message queues (RabbitMQ, Kafka), idempotent consumers, backpressure.",
                "Service discovery & configuration: Consul, etcd, environment config patterns.",
                "Resilience patterns: retries with backoff, circuit breakers, bulkheads.",
                "Monitoring & SRE basics: SLIs/SLOs, alerts, runbooks, capacity planning."
            }
        },
        {"SECURITY & BEST PRACTICES",
            {
                "OWASP top 10: SQLi, XSS, CSRF mitigation and secure coding practices.",
                "Secrets management: vaults, environment separation, rotation.",
                "Data protection: encryption at rest & in transit, GDPR/PII considerations.",
                "CI/CD security: scanning images, signed artifacts, least-privilege deployment.",
                "Incident response: triage, root-cause analysis, postmortems with blameless culture."
            }
        }
    });

    // FULL STACK
    add("Full Stack", {
        {"FOUNDATIONS",
            {
                "Strong command of both frontend and backend fundamentals (see Frontend & Backend phases).",
                "Understand HTTP, REST, websockets, and browser-server interactions.",
                "Version control workflows: git branching models, rebasing, CI integration."
            }
        },
        {"INTEGRATION & DATA FLOW",
            {
                "Design full request flow: client -> API -> DB -> cache -> client.",
                "Authentication flows: sessions, JWT, refresh tokens; secure cookie handling.",
                "Form handling & validation (client and server); file uploads, streaming large files.",
                "GraphQL basics: schemas, resolvers, N+1 problem and batching."
            }
        },
        {"DEPLOYMENT & DEVOPS",
            {
                "Containerization: Dockerfiles, multi-stage builds, image size optimization.",
                "Deploy pattern: blue/green, canary, rolling updates.",
                "CI/CD pipelines for full-stack apps: test matrix, staging environment, smoke tests.",
                "Observability: combine frontend and backend traces to see full-user journeys."
            }
        },
        {"ADVANCED TOPICS",
            {
                "Monorepo vs polyrepo: tooling, code sharing, release coordination.",
                "Microservices vs modular monolith: when and how to transition.",
                "Performance profiling across stack and end-to-end load testing.",
                "Security for full-stack: end-to-end threat modeling, secure defaults."
            }
        }
    });

    // DEVOPS
    add("DevOps", {
        {"CORE INFRASTRUCTURE",
            {
                "Linux system administration: users, processes, services, systemd, networking basics.",
                "Container orchestration: Kubernetes core concepts (Pods, Deployments, Services, ConfigMaps).",
                "Infrastructure as Code: Terraform, CloudFormation; manage state, modularization.",
                "CI/CD design: pipeline stages, artifact storage, secrets handling."
            }
        },
        {"OBSERVABILITY & SRE",
            {
                "Monitoring stacks: Prometheus + Grafana, exporters, and alerting rules.",
                "Logging: centralized logging (ELK/EFK), log retention, correlation IDs.",
                "Tracing: OpenTelemetry, instrumenting services, visualizing traces.",
                "SLO/SLA practices and error budgets; incident management & runbooks."
            }
        },
        {"SECURITY & NETWORK",
            {
                "Secure networking: VPC, subnetting, firewall rules, load balancers and ingress controllers.",
                "Service mesh basics: Istio/Linkerd — mTLS, traffic control, observability.",
                "CI/CD security: signed images, SBOM, vulnerability scanning.",
                "Secrets & credential management: Vault, cloud KMS, automatic rotation."
            }
        },
        {"COST & OPERATIONS",
            {
                "Cloud cost management: tagging, rightsizing, Reserved/Spot usage patterns.",
                "Autoscaling strategies: HPA/VPA, queue-based scaling, custom metrics.",
                "Disaster recovery planning: backups, multi-region failover, RTO/RPO estimations.",
                "Automation: build runbooks, automate repetitive tasks with IaC and scripting."
            }
        }
    });

    // DATA ANALYST
    add("Data Analyst", {
        {"DATA LITERACY",
            {
                "Understand data lifecycle: collection, cleaning, transformation, analysis, visualization.",
                "Excel & spreadsheets: pivot tables, advanced formulas, power queries.",
                "SQL for analytics: complex joins, window functions, CTEs, performance tuning.",
                "Statistics fundamentals: distributions, hypothesis testing, confidence intervals."
            }
        },
        {"TOOLS & VISUALIZATION",
            {
                "Visualization libraries & tools: Tableau, PowerBI, matplotlib, ggplot, seaborn.",
                "Data cleaning: pandas (Python) mastering groupby, merge, melt, pivot.",
                "Exploratory data analysis (EDA): outliers, missing values, feature engineering.",
                "Storytelling with data: choose correct charts, dashboards, and narrative flow."
            }
        },
        {"ADVANCED ANALYTICS",
            {
                "Time series analysis basics: decomposition, ARIMA, ETS models.",
                "A/B testing design and interpretation: sample size, power, p-hacking avoidance.",
                "SQL + Python integration: exporting queries, automating reports.",
                "Operationalizing reports: scheduled jobs, alerting on KPI thresholds."
            }
        }
    });

    // AI ENGINEER
    add("AI Engineer", {
        {"MATH & FUNDAMENTALS",
            {
                "Linear algebra essentials: vectors, matrices, eigenvalues, SVD applications.",
                "Probability & statistics: distributions, Bayes theorem, ML-specific metrics.",
                "Optimization basics: gradient descent variants, regularization techniques.",
                "Signal processing fundamentals (if applying to audio/vision): transforms, filters."
            }
        },
        {"ML PIPELINE & TOOLING",
            {
                "Data pipelines: ingestion, cleaning, feature stores, versioning data.",
                "Modeling frameworks: TensorFlow, PyTorch — build, train, save models.",
                "Experimentation: hyperparameter search, reproducibility, logging experiments (MLflow).",
                "Model evaluation: cross-validation, confusion matrices, ROC/AUC, calibration."
            }
        },
        {"PRODUCTION & MLOPS",
            {
                "Model deployment patterns: batch, online, streaming, and edge deployment.",
                "Model serving: TorchServe, TensorFlow Serving, or custom microservices.",
                "Monitoring models: data drift, concept drift, prediction quality monitoring.",
                "CI/CD for ML: model registry, automated retraining pipelines, canary rollouts."
            }
        }
    });

    // AI AND DATA SCIENTIST
    add("AI and Data Scientist", {
        {"FOUNDATIONAL SKILLS",
            {
                "Strong statistical inference and hypothesis testing skills.",
                "Feature engineering and domain feature creation practices.",
                "Model interpretability: SHAP, LIME, partial dependence plots.",
                "Experiment design and causal inference basics."
            }
        },
        {"MODELING & APPLICATION",
            {
                "Classical ML models: linear/logistic regression, trees, ensembles (XGBoost, LightGBM).",
                "Deep learning for structured data, images, text — when to use which.",
                "Natural Language Processing basics: tokenization, embeddings, transformers.",
                "Evaluation frameworks for business metrics: uplift, ROI."
            }
        },
        {"PRODUCTION & ETHICS",
            {
                "Ethics and bias mitigation: fairness metrics, data provenance.",
                "Reproducibility: seed control, environment pinning, data snapshots.",
                "Operationalization: deploy notebooks -> reproducible pipelines.",
                "Communicating results: craft dashboards and executive summaries."
            }
        }
    });

    // DATA ENGINEER
    add("Data Engineer", {
        {"DATA INFRASTRUCTURE",
            {
                "Batch vs stream processing: ETL patterns, Lambda / Kappa architectures.",
                "Data storage: columnar stores (Parquet), data lakes vs warehouses.",
                "Orchestration: Airflow, Dagster — DAG design, retries, idempotency.",
                "Messaging systems: Kafka, Kinesis — partitioning, retention, exactly-once considerations."
            }
        },
        {"PIPELINE QUALITY",
            {
                "Data contracts: schemas, evolution, Avro/Parquet and schema registries.",
                "Testing data pipelines: property-based tests, data diffing, golden datasets.",
                "Performance tuning: partitioning, clustering, file sizing, compaction.",
                "Security & governance: lineage, masking, RBAC, GDPR compliance."
            }
        },
        {"SCALE & OPERATIONS",
            {
                "Distributed compute: Spark optimization, memory management, shuffle tuning.",
                "Cloud-native data stacks: managed warehouses (Snowflake, BigQuery, Redshift).",
                "Cost optimization: query cost, storage tiering, lifecycle policies.",
                "Automation & monitoring: SLA checks, alerting for pipeline failures."
            }
        }
    });

    // ANDROID
    add("Android", {
        {"BASICS & KOTLIN",
            {
                "Kotlin fundamentals: null-safety, coroutines, extension functions.",
                "Android app fundamentals: activities, fragments, lifecycle, intents.",
                "UI: Jetpack Compose vs XML layouts — state hoisting & recomposition.",
                "App resources: localization, configuration changes, density buckets."
            }
        },
        {"ARCHITECTURE & DEPENDENCY",
            {
                "Android architecture: MVVM, Clean Architecture principles.",
                "Jetpack components: ViewModel, LiveData/StateFlow, Room persistence.",
                "Dependency injection: Hilt/Dagger — scopes and component lifecycles.",
                "Background work: WorkManager, foreground services, push notifications."
            }
        },
        {"PERFORMANCE & RELEASE",
            {
                "App size optimization: ProGuard/R8, resource shrinking, split apks.",
                "Testing: unit, instrumentation, UI tests with Espresso/Compose testing.",
                "CI/CD: Play Console integration, internal tracks, staged rollouts.",
                "Security: keystore management, secure storage, obfuscation strategies."
            }
        }
    });

    // MACHINE LEARNING
    add("Machine Learning", {
        {"MATH & THEORY",
            {
                "Linear algebra and multivariate calculus basics for ML.",
                "Probability, statistics and information theory essentials.",
                "Learning theory: bias-variance tradeoff, VC dimension concepts.",
                "Optimization: gradient methods, second-order methods, schedule tuning."
            }
        },
        {"MODELING",
            {
                "Supervised learning: regression, classification, ensembles.",
                "Unsupervised learning: clustering, dimensionality reduction (PCA, t-SNE, UMAP).",
                "Neural networks: architectures, regularization, batch norm.",
                "Sequence models: RNNs, LSTM, Transformers for sequential tasks."
            }
        },
        {"DEPLOYMENT & ETHICS",
            {
                "Productionizing models: serialization, serving, feature pipelines.",
                "Fairness and privacy: differential privacy basics, de-identification.",
                "Monitoring: drift detection, shadow deployments, rollback strategies.",
                "Interpretability: model-agnostic techniques and business translation."
            }
        }
    });

    // POSTGRESQL
    add("PostgreSQL", {
        {"CORE CONCEPTS",
            {
                "Relational model refresher: keys, normalization, ACID properties.",
                "SQL mastery: advanced joins, window functions, recursive CTEs.",
                "Indexes: B-tree, GIN, GiST, partial and expression indexes.",
                "Transactions & isolation levels: read phenomena and tuning."
            }
        },
        {"PERFORMANCE & OPERATIONS",
            {
                "Query planning & EXPLAIN ANALYZE use; optimizing expensive queries.",
                "Partitioning strategies: range/list/hash partitioning and maintenance.",
                "VACUUM & autovacuum tuning, bloat detection and mitigation.",
                "High availability: streaming replication, failover automation, PITR backups."
            }
        },
        {"EXTENSIONS & ECOSYSTEM",
            {
                "Useful extensions: PostGIS, pg_cron, pg_stat_statements, citus for scaling.",
                "Connection pooling: PgBouncer, Pgpool-II; sizing pool and preventing overload.",
                "Security: roles, row-level security (RLS), encryption at rest.",
                "Schema migration and deployment strategies for production."
            }
        }
    });

    // iOS
    add("iOS", {
        {"SWIFT & LANGUAGE",
            {
                "Swift language deep dive: optionals, protocols, generics, ARC behavior.",
                "Xcode proficiency: instruments, debugging, scheme management.",
                "UIKit vs SwiftUI: lifecycle, state management, performance tradeoffs.",
                "Concurrency: async/await, GCD, operation queues, and structured concurrency."
            }
        },
        {"APP ARCHITECTURE",
            {
                "Architectures: MVC, MVVM, VIPER; choose pragmatic patterns.",
                "Persistence: Core Data, SQLite, Keychain for secure storage.",
                "Networking: URLSession, background fetch, large download handling.",
                "Testing: unit, UI tests, snapshot testing, TestFlight for beta distribution."
            }
        },
        {"SECURITY & RELEASE",
            {
                "App signing, provisioning profiles, certificates and release pipelines.",
                "App Store guidelines and compliance; privacy disclosure best practices.",
                "Performance tuning: Instruments for CPU, memory, energy impact.",
                "Localization and accessibility: VoiceOver, Dynamic Type, UI tests."
            }
        }
    });

    // BLOCKCHAIN
    add("Blockchain", {
        {"CRYPTO FUNDAMENTALS",
            {
                "Cryptography basics: hashing, public-key cryptography, digital signatures.",
                "Distributed ledger fundamentals: blocks, consensus, forks.",
                "Smart contract platforms overview: Ethereum, Solana, layer-2s."
            }
        },
        {"SMART CONTRACTS",
            {
                "Solidity basics: types, gas model, reentrancy and vulnerabilities.",
                "Formal verification and audits: static analysis tools and practices.",
                "Testing & tooling: Hardhat/Truffle, ganache, unit testing contracts.",
                "Security patterns: upgradable proxies, safe initialization, access control."
            }
        },
        {"ECOSYSTEM & UX",
            {
                "Token standards: ERC-20, ERC-721, ERC-1155, metadata standards.",
                "Wallets and UX: Metamask integration, gas UX, deterministic wallets.",
                "On-chain vs off-chain tradeoffs and scaling solutions (rollups, sharding).",
                "Regulatory and legal considerations: KYC/AML, token compliance basics."
            }
        }
    });

    // QA
    add("QA", {
        {"TEST FOUNDATIONS",
            {
                "Testing pyramid: unit, integration, E2E — when to use which level.",
                "Test design techniques: equivalence partitioning, boundary testing, pairwise.",
                "Automation frameworks: Selenium, Playwright, Cypress, and where they fit."
            }
        },
        {"PRACTICAL TESTING",
            {
                "Mocking and stubbing: isolate dependencies for reliable unit tests.",
                "CI integration: run tests in pipeline, flaky test detection and quarantine.",
                "Performance and load testing: JMeter, k6; setting realistic SLAs and thresholds."
            }
        },
        {"QUALITY ENGINEERING",
            {
                "Shift-left testing: integrate QA earlier into design & development.",
                "Observability-driven QA: leveraging logs and traces to find issues.",
                "Test data management and environment provisioning strategies."
            }
        }
    });

    // SOFTWARE ARCHITECT
    add("Software Architect", {
        {"CORE PRINCIPLES",
            {
                "System decomposition and modularization principles.",
                "Architectural styles: layered, event-driven, microservices, hexagonal.",
                "Non-functional requirements: performance, reliability, scalability and security tradeoffs."
            }
        },
        {"DESIGN & GOVERNANCE",
            {
                "Define clear component contracts and versioning strategies.",
                "Data ownership and bounded contexts in domain-driven design.",
                "Operational concerns: deployment patterns, monitoring, and cost controls."
            }
        },
        {"COMMUNICATION",
            {
                "Create architecture docs, decision records (ADR), and run workshops.",
                "Mentor engineering teams on tradeoffs and design patterns.",
                "Establish governance for dependencies, shared libraries and infra."
            }
        }
    });

    // CYBER SECURITY
    add("Cyber Security", {
        {"FOUNDATIONS",
            {
                "Networking security basics: firewalls, IDS/IPS, VPNs.",
                "Cryptography essentials: symmetric/asymmetric, key management.",
                "Threat modeling: STRIDE, attack surface, threat trees."
            }
        },
        {"APPLIED PRACTICE",
            {
                "Web app security: OWASP top 10, secure coding, input validation.",
                "Incident response: detection, containment, eradication, recovery.",
                "Red team / blue team exercises, vulnerability assessments and pentests."
            }
        },
        {"GOVERNANCE",
            {
                "Security frameworks: NIST, ISO27001, SOC2 basics.",
                "Identity & access management: SSO, MFA, RBAC, least privilege.",
                "Compliance & privacy: GDPR, data classification, secure logging."
            }
        }
    });

    // UX DESIGN
    add("UX Design", {
        {"FOUNDATIONS",
            {
                "User research: interviews, surveys, contextual inquiry.",
                "Design thinking: empathy, define, ideate, prototype, test.",
                "Interaction design: user flows, wireframes, information architecture."
            }
        },
        {"VISUAL & PROTOTYPING",
            {
                "Prototyping tools: Figma, Sketch, Adobe XD — components and design systems.",
                "Usability testing: moderated & unmoderated tests, analyze sessions.",
                "Accessibility: WCAG guidelines, keyboard navigation, color contrast."
            }
        },
        {"DELIVERY",
            {
                "Handoff to engineering: specs, tokens, design system docs.",
                "Measure UX: usability metrics, funnel analysis, session replay.",
                "Iterative design: A/B tests, feature flags, continuous improvement."
            }
        }
    });

    // TECHNICAL WRITER
    add("Technical Writer", {
        {"FOUNDATIONS",
            {
                "Audience analysis and documentation planning.",
                "Writing clear how-tos, tutorials, and reference docs.",
                "Versioning docs and using source control for docs (docs-as-code)."
            }
        },
        {"TOOLS & PRACTICE",
            {
                "Markdown, AsciiDoc, Docusaurus, MkDocs; build documentation pipelines.",
                "API docs: OpenAPI/Swagger, code examples, interactive docs.",
                "Localization and docs internationalization workflow."
            }
        },
        {"COLLABORATION",
            {
                "Work with engineers to keep docs in sync with code and releases.",
                "Measure docs success: search analytics, support ticket correlation.",
                "Include tutorials, onboarding flows, and quick-start guides."
            }
        }
    });

    // GAME DEVELOPER
    add("Game Developer", {
        {"CORE SKILLS",
            {
                "Math for games: linear algebra, transforms, collision detection.",
                "Game engines: Unity (C#) or Unreal (C++/Blueprints) fundamentals.",
                "Gameplay architecture: entity-component systems, game loops, fixed timestep."
            }
        },
        {"ASSETS & PIPELINES",
            {
                "Asset pipelines: importing, LODs, texture atlases, animation rigs.",
                "Physics: rigid bodies, collision layers, optimization for many objects.",
                "Networking for games: authoritative servers vs peer-to-peer, latency handling."
            }
        },
        {"OPTIMIZATION & RELEASE",
            {
                "Performance profiling: frame budgets, draw calls, batching.",
                "Platform-specific deployment: consoles, PC, mobile optimizations.",
                "Live ops: telemetry, matchmaking, anti-cheat basics."
            }
        }
    });

    // SERVER SIDE GAME DEVELOPER
    add("Server Side Game Developer", {
        {"NETWORKING",
            {
                "UDP vs TCP for game networking; reliability layers on UDP.",
                "State synchronization: snapshots, delta compression, interpolation.",
                "Authoritative server design: cheat prevention, reconciliation."
            }
        },
        {"SCALE & PERSISTENCE",
            {
                "Matchmaking and session management: scales, affinity, sharding.",
                "Realtime messaging: pub/sub, Redis, scalable gateways.",
                "Persistence: player state, leaderboards, anti-cheat logging."
            }
        },
        {"OPERATIONS",
            {
                "Autoscaling real-time servers, latency SLOs, DDoS protections.",
                "Live updates and migrations with minimal downtime.",
                "Monitoring: player experience metrics, server tick rates, error budgets."
            }
        }
    });

    // MLOPS
    add("MLOps", {
        {"INFRA & PIPELINES",
            {
                "Reproducible training pipelines: containerized environments, data snapshots.",
                "Feature stores and model registries: versioning features and models.",
                "Orchestration for training and retraining: Airflow, Kubeflow, Argo."
            }
        },
        {"SERVING & MONITORING",
            {
                "Low-latency serving: batching, model quantization, model optimization.",
                "Monitoring ML: prediction distributions, drift detection, alerting.",
                "Governance: lineage, explainability, and reproducible audits."
            }
        },
        {"COST & OPERATIONS",
            {
                "Cost-aware ML: cloud GPU management, spot/spot-fleet considerations.",
                "Security for ML: secrets for models, secure model stores.",
                "Automation: retrain pipelines, canary deployments for models."
            }
        }
    });

    // PRODUCT MANAGER
    add("Product Manager", {
        {"FOUNDATIONS",
            {
                "Understanding customer: problem interviews, personas, JTBD (Jobs To Be Done).",
                "Prioritization frameworks: RICE, MoSCoW, Kano; measuring impact vs effort.",
                "Roadmapping: outcomes vs outputs; writing clear product specs."
            }
        },
        {"DELIVERY & EXECUTION",
            {
                "Working with engineering: sprint planning, definition of done, release cadence.",
                "OKRs and metrics: define success metrics and instrumentation needs.",
                "User research & validation: prototypes, beta programs, and feedback loops."
            }
        },
        {"STRATEGY",
            {
                "Competitive analysis, go-to-market planning, pricing basics.",
                "Stakeholder management and cross-functional leadership.",
                "Scaling roadmap: system constraints, legal/regulatory considerations."
            }
        }
    });

    // ENGINEERING MANAGER
    add("Engineering Manager", {
        {"PEOPLE MANAGEMENT",
            {
                "Hiring: writing job specs, interviewing, and calibration.",
                "Onboarding and career ladders: mentoring, 1:1s, performance reviews.",
                "Team health: burnout prevention, capacity planning, team rituals."
            }
        },
        {"TECHNICAL LEADERSHIP",
            {
                "Architectural direction: trade-offs, tech debt, and roadmaps.",
                "Operational excellence: CI, monitoring, incident response readiness.",
                "Cross-team collaboration and alignment."
            }
        },
        {"GROWTH & CULTURE",
            {
                "Promote learning culture: brown-bags, book clubs, hackweeks.",
                "Establish inclusive hiring and feedback-driven culture.",
                "Measure team outcomes: throughput, quality, and customer impact."
            }
        }
    });

    // DEVELOPER RELATIONS
    add("Developer Relations", {
        {"FOUNDATIONS",
            {
                "Understand product: SDKs, APIs, developer experience pain points.",
                "Content creation: tutorials, example apps, sample code.",
                "Community building: hackathons, meetups, developer slack/discord moderation."
            }
        },
        {"TECH ADVOCACY",
            {
                "Create onboarding flows and sample projects that 'wow' new devs.",
                "Collect developer feedback loops and surface to product teams.",
                "Metrics: developer activation, retention, and NPS for dev experience."
            }
        },
        {"OPERATIONS",
            {
                "Manage developer programs: advocate programs, community grants.",
                "Public speaking: talks, workshops, webinars best practices.",
                "Open source stewardship and sample code maintenance."
            }
        }
    });

    // BI ANALYST
    add("BI Analyst", {
        {"CORE SKILLS",
            {
                "SQL at scale: efficient reporting queries, materialized views, window functions.",
                "Data modeling for BI: star schema, slowly changing dimensions.",
                "ETL/ELT pipelines: reliable refreshes, incremental loads, monitoring."
            }
        },
        {"VISUALIZATION & DELIVERY",
            {
                "Dashboards: design for insights, refresh cadence, row-level security.",
                "Self-serve analytics: empower product teams with curated datasets.",
                "Automation: scheduled reports, data alerts, embedding analytics."
            }
        },
        {"GOVERNANCE & STRATEGY",
            {
                "Data governance: single source of truth, metric stores.",
                "Cross-functional partnership: product, finance, ops alignment.",
                "KPIs & causal analysis: linking data to business outcomes."
            }
        }
    });

    // End of definitions
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    initRoadmaps();

    // Print welcome banner with endl for flush
    cout << "============================================================" << endl;
    cout << "              ROLE-BASED ROADMAP SEARCH SYSTEM" << endl;
    cout << "============================================================" << endl << endl;

    cout << "Available Roles:" << endl;
    cout << "------------------------------------------------------------" << endl;
    for (auto &kv : roadmaps) {
        cout << kv.second.displayName << endl;
    }
    cout << "------------------------------------------------------------" << endl;
    cout << "Type any role name to view its FULL roadmap." << endl;
    cout << "(To exit, type: exit)" << endl << endl;

    while (true) {
        cout << "\nEnter role: " << flush;  // flush ensures prompt is visible immediately
        string line;
        if (!getline(cin, line)) break;

        // trim input
        while (!line.empty() && isspace((unsigned char)line.front())) line.erase(line.begin());
        while (!line.empty() && isspace((unsigned char)line.back())) line.pop_back();
        if (line.empty()) continue;

        string lower = normalize(line);
        if (lower == "exit" || lower == "quit") {
            cout << "Bye." << endl;
            break;
        }

        if (lower == "list") {
            cout << "\nSupported roles:" << endl;
            vector<string> names;
            for (auto &kv : roadmaps) names.push_back(kv.second.displayName);
            sort(names.begin(), names.end());
            for (auto &n : names) cout << " - " << n << endl;
            continue;
        }

        vector<string> matches = findMatches(line);
        if (matches.empty()) {
            cout << "No roadmap found for '" << line << "'. Try 'list' to see supported roles or type a substring." << endl;
            continue;
        }

        if (matches.size() > 1) {
            cout << "\nMultiple matches found. Please choose:" << endl;
            for (size_t i = 0; i < matches.size(); ++i)
                cout << i + 1 << ") " << roadmaps[matches[i]].displayName << endl;
            cout << "Enter number (or 0 to cancel): " << flush;

            string numLine;
            if (!getline(cin, numLine)) break;
            int choice = 0;
            try { choice = stoi(numLine); } catch (...) { choice = 0; }
            if (choice <= 0 || choice > (int)matches.size()) {
                cout << "Cancelled." << endl;
                continue;
            }
            printRoadmap(roadmaps[matches[choice - 1]]);
        } else {
            printRoadmap(roadmaps[matches[0]]);
        }
    }

    return 0;
}