/**
 * Ajouts professionnels pour ICPocket
 * Animation, interactions et fonctionnalités avancées
 */

document.addEventListener('DOMContentLoaded', function() {
    // 1. Effet de défilement sur la navigation
    window.addEventListener('scroll', function() {
        const nav = document.querySelector('nav');
        if (window.scrollY > 50) {
            nav.classList.add('scrolled');
        } else {
            nav.classList.remove('scrolled');
        }
    });
    
    // 2. Système de notification
    window.showNotification = function(message, type = 'info', duration = 5000) {
        // Créer la notification
        const notification = document.createElement('div');
        notification.className = `notification ${type}`;
        notification.innerHTML = `
            <button class="close-btn"><i class="fas fa-times"></i></button>
            <div class="notification-content">${message}</div>
        `;
        
        // Ajouter au DOM
        document.body.appendChild(notification);
        
        // Afficher avec animation
        setTimeout(() => notification.classList.add('show'), 10);
        
        // Configurer le bouton de fermeture
        const closeBtn = notification.querySelector('.close-btn');
        closeBtn.addEventListener('click', () => {
            notification.classList.remove('show');
            setTimeout(() => notification.remove(), 500);
        });
        
        // Auto-fermeture après durée spécifiée
        if (duration) {
            setTimeout(() => {
                notification.classList.remove('show');
                setTimeout(() => notification.remove(), 500);
            }, duration);
        }
        
        return notification;
    };
    
    // 3. Remplacer les animations de chargement par des skeletons quand c'est pertinent
    function replaceLoadingAnimations() {
        const loadingElements = document.querySelectorAll('.loading-animation');
        
        loadingElements.forEach(element => {
            // Vérifier si on est dans un conteneur de releases ou commits
            const isReleaseContainer = element.closest('#releases-container');
            const isCommitContainer = element.closest('#commits-container');
            
            if (isReleaseContainer || isCommitContainer) {
                // Déterminer le nombre de squelettes à créer
                const count = isReleaseContainer ? 2 : 3;
                const container = document.createElement('div');
                container.className = 'skeleton-container';
                
                // Créer les squelettes de cartes
                for (let i = 0; i < count; i++) {
                    const skeletonCard = document.createElement('div');
                    skeletonCard.className = 'skeleton-card';
                    skeletonCard.innerHTML = `
                        <div class="skeleton-title"></div>
                        <div class="skeleton-text"></div>
                        <div class="skeleton-text"></div>
                        <div class="skeleton-text"></div>
                    `;
                    container.appendChild(skeletonCard);
                }
                
                // Remplacer l'animation de chargement par les squelettes
                element.parentNode.replaceChild(container, element);
            }
        });
    }
    
    // Appliquer le remplacement après un court délai
    setTimeout(replaceLoadingAnimations, 100);
    
    // 4. Animations à l'apparition des éléments (Intersection Observer)
    const animateOnScroll = function() {
        const observer = new IntersectionObserver((entries) => {
            entries.forEach(entry => {
                if (entry.isIntersecting) {
                    entry.target.classList.add('visible');
                    observer.unobserve(entry.target);
                }
            });
        }, {
            threshold: 0.1
        });
        
        // Observer les éléments avec la classe .animate-on-scroll
        document.querySelectorAll('.animate-on-scroll').forEach(el => {
            observer.observe(el);
            // Ajouter une classe pour le style initial (invisible)
            el.classList.add('to-animate');
        });
    };
    
    // Initialiser l'animation au défilement
    animateOnScroll();
    
    // 5. Améliorer les boutons avec effet de clic
    document.querySelectorAll('button, .btn-primary, .btn-secondary, .download-button').forEach(button => {
        button.addEventListener('mousedown', function() {
            this.style.transform = 'scale(0.98)';
        });
        
        button.addEventListener('mouseup', function() {
            this.style.transform = '';
        });
        
        button.addEventListener('mouseleave', function() {
            this.style.transform = '';
        });
    });
    
    // 6. Ajouter la classe modern-card aux cartes existantes
    document.querySelectorAll('.release-card, .commit-card, .news-card').forEach(card => {
        card.classList.add('modern-card');
    });
    
    // 7. Ajouter des classes d'animation aux sections
    document.querySelectorAll('section').forEach((section, index) => {
        section.classList.add('fade-in');
        section.style.animationDelay = `${index * 0.1}s`;
        
        // Rendre les enfants directs séquentiels
        if (section.children.length > 1) {
            const contentContainer = section.querySelector('div');
            if (contentContainer) contentContainer.classList.add('stagger-children');
        }
    });
    
    // Exemple d'utilisation de la notification
    // Commenter cette ligne en production
    // setTimeout(() => showNotification('Bienvenue sur le site ICPocket !', 'info'), 1000);
    
    // Marquer le lien actif dans la navigation
    const currentPage = window.location.pathname.split('/').pop();
    const navLinks = document.querySelectorAll('nav button a');
    
    navLinks.forEach(link => {
        const linkHref = link.getAttribute('href');
        if (linkHref === currentPage || 
            (currentPage === '' && linkHref === 'index.html')) {
            link.classList.add('active');
            link.parentElement.classList.add('current-page');
        }
    });
    
    // Ajouter la navigation par "retour en haut" pour les longues pages
    const scrollToTopButton = document.createElement('button');
    scrollToTopButton.className = 'scroll-top-button';
    scrollToTopButton.innerHTML = '<i class="fas fa-arrow-up"></i>';
    scrollToTopButton.setAttribute('aria-label', 'Retour en haut de page');
    document.body.appendChild(scrollToTopButton);
    
    scrollToTopButton.addEventListener('click', () => {
        window.scrollTo({
            top: 0,
            behavior: 'smooth'
        });
    });
    
    // Afficher/masquer le bouton selon la position de défilement
    window.addEventListener('scroll', () => {
        if (window.scrollY > 300) {
            scrollToTopButton.classList.add('visible');
        } else {
            scrollToTopButton.classList.remove('visible');
        }
    });
    
    // Ajouter des transitions de page
    document.addEventListener('click', e => {
        const link = e.target.closest('a');
        
        if (link && 
            !link.hasAttribute('target') && 
            link.hostname === window.location.hostname && 
            link.getAttribute('href').indexOf('#') !== 0 && 
            !link.hasAttribute('download')) {
            
            e.preventDefault();
            const targetHref = link.getAttribute('href');
            
            document.body.classList.add('page-transition-out');
            
            setTimeout(() => {
                window.location.href = targetHref;
            }, 300);
        }
    });
    
    // Ajout: Traitement des événements d'erreur pour les images
    document.querySelectorAll('img').forEach(img => {
        img.addEventListener('error', function() {
            this.src = 'img/image-placeholder.jpg';
            this.classList.add('img-error');
        });
    });
    
    // Ajout: Chargement paresseux des images
    if ('IntersectionObserver' in window) {
        const imageObserver = new IntersectionObserver((entries) => {
            entries.forEach(entry => {
                if (entry.isIntersecting) {
                    const img = entry.target;
                    if (img.dataset.src) {
                        img.src = img.dataset.src;
                        img.removeAttribute('data-src');
                        imageObserver.unobserve(img);
                    }
                }
            });
        });
        
        document.querySelectorAll('img[data-src]').forEach(img => {
            imageObserver.observe(img);
        });
    }
    
    // Ajout: Notifications de dernière minute (à montrer une seule fois par session)
    if (!sessionStorage.getItem('notificationShown')) {
        setTimeout(() => {
            showNotification('Bienvenue sur le site ICPocket ! Découvrez notre dernière version.', 'info', 8000);
            sessionStorage.setItem('notificationShown', 'true');
        }, 2000);
    }
    
    // Initialisation du mode A/B testing
    initABTesting();
});

// Ajout: Fonction pour A/B testing
function initABTesting() {
    // Exemple: tester deux variantes de bouton de téléchargement
    if (document.getElementById('downloadButton')) {
        // Générer aléatoirement la variante A ou B
        const downloadBtn = document.getElementById('downloadButton');
        downloadBtn.classList.add('download-button');
        downloadBtn.innerHTML = '<i class="fas fa-rocket"></i><span>Télécharger v0.9</span>';
    }
} 