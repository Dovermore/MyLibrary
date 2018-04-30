import tensorflow as tf
import numpy as np
from sklearn.base import BaseEstimator, ClassifierMixin
from sklearn.exceptions import NotFittedError

class DNNClassifier(BaseEstimator, ClassifierMixin):
    """
    Brief: This class implements the methods with sklearn compatible 
           structure,and is aimed to enable training with sklearn 
           RandomizedSearchCV class of sklearn.
           Available for one-hot encoded labels.
    
    Authorship: Calvin Zhuoqun Huang, 19 Jan 2018
    
    Requirements: tensorflow 1.4 and above
                  numpy 1.13 and above
                  sklearn 0.19 and above
                  
    Known issues: A lot :D
                  Accuracy implementation is not entirely correct
    """
    
    def __init__(self, n_hidden_layers=5, n_neurons=100, 
                 optimizer_class=tf.train.AdamOptimizer,
                 learning_rate=0.01, n_epochs=100, batch_size=50, 
                 activation=tf.nn.selu, initializer=None,
                 batch_norm_momentum=None, dropout_rate=None, 
                 regularizer=None, reg_scale=0.001, base_dir="temp", 
                 log_freq=5, max_stop=5,
                 random_state=None):
        """
        Parameters:
        n_hidden_layers: 
            Type: int, default to 5
            Number of neural network layers you wish to use

        n_neurons: 
            Type: int or sequence type containing int, default to 100.
            Number of neurons you wish to use in each layer.
            If integer is passed, then implements constant number of
            neuron each layer


        optimizer_class: 
            Type: default to tf.train.AdamOptimizer
            Optimizer class from tensorflow train optimizer or any
            other compatible optimizer


        learning_rate: 
            Type: float, default to 0.01
            Learning rate of the neural network.
        
        n_epochs:
            Type: int, defaults to 100
            The number of full epoches get used to train the model
        
        batch_size:
            Type: int, defaults to 50
            The number of examples in each batch
            
        activation:
            Type: function, defaults to tf.nn.selu
            The activation functions used in hidden layers
            
        batch_norm_momentum:
            Type: 0 < float < 1, defaults to None
            When given, applies batch normalization to the model
            
        dropout_rate:
            Type: 0 < float < 1, defaults to None
            When given, applies dropout regularization to the model
        
        regularizer:
            Type: defaults to None
            The type of regularization applied to hidden layer.
            Supports tf.contrib.layers.l2/l1regularizer.
        
        reg_scale:
            Type: float, defaults to 0.001
            paramter beta for regularization.
            
        base_dir:
            Type: str or os.path type
            The directory you wantted to store the log file and session
            record(maxima 20). With unique timestamp on the folder and
            Record number on the files.
            
        log_freq:
            Type: int, defaults to 5
            The interval of epochs for storing logs and session parameters
            
        max_stop:
            Type: int, defaults to 5
            Specify the number of intervals before early stopping is 
            happens if both the accuracy and loss on
            validation set drops belows the best mark for that long interval
            (i.e. max_stop * log_freq)
            (Only triggers if during fit stage the validation set X and y
            are given)
            
        random_state:
            Type: int, defaults to None
            Random state for tensorflow random and numpy random.
            
            
        Return: None
        """
        self.n_hidden_layers = n_hidden_layers
        self.n_neurons = n_neurons
        self.optimizer_class = optimizer_class
        self.learning_rate = learning_rate
        self.n_epochs = n_epochs
        self.batch_size = batch_size
        self.activation = activation
        self.initializer= initializer
        self.batch_norm_momentum = batch_norm_momentum
        self.dropout_rate = dropout_rate
        self.regularizer = regularizer
        self.reg_scale = reg_scale
        self.base_dir = base_dir
        self.log_freq = log_freq
        self.max_stop = max_stop
        self.random_state = random_state
        self._session = None
        self._graph = None
#         clear the local graph
        tf.reset_default_graph()
        
    def _dnn_layout(self, inputs, n_labels):
        """
        Constructs the deep neural network part of the model
        
        Parameters:
        inputs: 
            The inputs tensor for deep NN
            
        n_labels:
            The output label number of NN
            
        
        Return: logits from the last tensor
        
        TensorName:
            dropout1...n, hidden1...n, bn1...n 
        """
        if type(self.n_neurons) is int:
#             convert the number of neurons to a generator
            self._n_neurons = (self.n_neurons for i in 
                               range(self.n_hidden_layers))
        elif type(self.n_neurons) is list:
            self._n_neurons = iter(self.n_neurons)
        else:
            raise TypeError("The type of n_neurons can't be %s" 
                            % str(type(self.n_neurons)))
        with tf.name_scope("dnn"):
            for lay in range(1, self.n_hidden_layers+1):
                if self.dropout_rate:
#                     Dropout layer
                    inputs = tf.layers.dropout(
                                      inputs, 
                                      rate=self.dropout_rate, 
                                      training=self._training, 
                                      name="dropout%d" % lay
                                    )
#                 Hidden layer
                inputs = tf.layers.dense(
                                    inputs,
                                    next(self._n_neurons),
                                    activation=self.activation,
                                    kernel_initializer=self.initializer(),
                                    kernel_regularizer=
                                        self.regularizer(self.reg_scale),
                                    trainable=True,
                                    name="hidden%d" % lay
                                )
                if self.batch_norm_momentum:
#                     BN layer
                    inputs = tf.layers.batch_normalization(
                                        inputs,
                                        momentum=self.batch_norm_momentum,
                                        training=self._training,
                                        name="bn%d" % lay
                                    )
            logits = tf.layers.dense(
                                    inputs,
                                    n_labels,
                                    name="logits"
                                )
            return logits

    def _build_graph(self, n_features, n_labels):
        """Build the full model with all units"""
        if self.random_state is not None:
            tf.set_random_seed(self.random_state)
            np.random.seed(self.random_state)
        self._graph = tf.Graph()
        self._graph.as_default()
        
        X = tf.placeholder(
                    tf.float32,
                    shape=(None, n_features),
                    name="X"
                )
        y = tf.placeholder(
                    tf.float32,
                    shape=(None, n_labels),
                    name="y"
                )

        self._training = tf.placeholder_with_default(False, None, 
                                                     name="training")
        
        logits = self._dnn_layout(X, n_labels)
        y_proba = tf.nn.softmax(logits, name="y_proba")
        
        with tf.name_scope("loss"):
            xentropy = tf.nn.softmax_cross_entropy_with_logits(
                                labels=y, logits=logits, name="xentropy")
            base_loss = tf.reduce_mean(xentropy)
            reg_loss = tf.get_collection(tf.GraphKeys.REGULARIZATION_LOSSES)
            loss = tf.add_n([base_loss]+reg_loss, name="loss")
            
        with tf.name_scope("train"):
            batch_updates = tf.get_collection(tf.GraphKeys.UPDATE_OPS)
            optimizer = self.optimizer_class(self.learning_rate)
            training_op = optimizer.minimize(loss)
            
        with tf.name_scope("eval"):
            true_labels = tf.argmax(y, axis=1)
            y_pred = tf.argmax(logits, axis=1)
            all_acc, acc = tf.metrics.accuracy(labels=true_labels, 
                                               predictions=y_pred)
            
        time_now = datetime.now().strftime("%Y%m%d%H%M%S")
        save_dir = os.path.join(self.base_dir, "saves", time_now)
        log_dir = os.path.join(self.base_dir, "logs", time_now)
#         if the path for save and log don't exist, create them.
        if not os.path.isdir(save_dir):
            os.makedirs(save_dir)
        if not os.path.isdir(log_dir):
            os.makedirs(log_dir)
        with tf.name_scope("savNlog"):
            saver = tf.train.Saver(max_to_keep=20)
            file_writer = tf.summary.FileWriter(log_dir)
            train_acc_log = tf.summary.scalar("train_accuracy", acc)
            validation_acc_log = tf.summary.scalar("validation_accuracy", acc)
            train_loss_log = tf.summary.scalar("train_loss", loss)
            validation_loss_log = tf.summary.scalar("validation_loss", loss)


        with tf.name_scope("initializer"):
#             Bundle the two initializers together
            inits = tf.group(tf.global_variables_initializer(),
                                    tf.local_variables_initializer())
            
        
        self._X = X
        self._y = y
        self._y_proba = y_proba
        self._logits = logits
        self._xentropy = xentropy
        self._loss = loss
        self._batch_updates = batch_updates
        self._y_pred = y_pred
        self._optimizer = optimizer
        self._training_op = training_op
        self._true_labels = true_labels
        self._acc = acc
        self._save_dir = save_dir
        self._log_dir = log_dir
        self._saver = saver
        self._file_writer = file_writer
        self._train_acc_log = train_acc_log
        self._validation_acc_log = validation_acc_log
        self._train_loss_log = train_loss_log
        self._validation_loss_log = validation_loss_log
        self._inits = inits

    def close_session(self):
        """
        Close the session
        """
        if self._session:
            self._session.close()
            
    def _get_batch(self, X, y, batch_size, epoch, batch):
        """
        get a batch of training set
        """
        num_inst = X.shape[0]
        np.random.seed(epoch * batch * np.random.randint(100))
        shuffle_index = np.random.randint(num_inst, size=batch_size)
        return X[shuffle_index], y[shuffle_index]

    def fit(self, X, y, X_val=None, y_val=None, model="loss"):
        """
        Parameters:
        X:
            Training sets' data of shape [n_example, n_feature]
        y:
            Training sets' labels of shape [n_example, n_labels]
        X_val:
            Valiadtion sets data used for early stopping
        y_val:
            Valiadtion sets labels used for early stopping
        model:
            string: "loss" or "acc"
            in case of early stopping, restore the model with best
            loss on validation set or model with best accuracy on 
            validation set
            
        Return: None
        """
        self.close_session()
        if self._graph:
            print("Resetting Old Graph")
            tf.reset_default_graph()
        
        n_features = X.shape[1]
        n_labels = y.shape[1]
        self._build_graph(n_features, n_labels)
        
        training_size = X.shape[0]
        self._file_writer.add_graph(tf.get_default_graph())
        
        if X_val is not None and y_val is not None:
            max_val_loss = np.inf
            count_drop_loss = 0
            self.best_model_loss_ = None

            max_val_acc = 0
            count_drop_acc = 0
            self.best_model_acc_ = None

        sess = tf.Session()
        self._session = sess
        sess.as_default()
        sess.run(self._inits)
        for epoch in range(1, self.n_epochs+1):
            for batch in range(1, np.ceil(training_size/
                                          self.batch_size).astype(int)+1):
                
                X_batch, y_batch =  self._get_batch(X, y, self.batch_size, 
                                                    epoch, batch)

                sess.run([self._training_op, self._batch_updates], 
                         feed_dict={self._X:X_batch, 
                                    self._y: y_batch, 
                                    self._training:True})
            if not epoch % self.log_freq:
                train_acc, train_loss, log_train_acc, log_train_loss = sess.run(
                                                    [self._acc, 
                                                     self._loss, 
                                                     self._train_acc_log, 
                                                     self._train_loss_log], 
                                                    feed_dict={self._X: X, self._y: y})

                self._file_writer.add_summary(log_train_acc, epoch)


                save_file_path = self._saver.save(sess, 
                                    os.path.join(self._save_dir, 
                                        "{}.ckpt".format(epoch//
                                                         self.log_freq)))
                
                self.last_epoch_model = save_file_path
                
                if X_val is not None and y_val is not None:
                    val_acc, val_loss, log_val_acc, log_val_loss = sess.run(
                                                    [self._acc,  
                                                     self._loss,
                                                     self._validation_acc_log,
                                                     self._validation_loss_log],
                                                    feed_dict={self._X: X_val, self._y: y_val})
                    self._file_writer.add_summary(log_val_acc, epoch)

                    if val_loss < max_val_loss:
                        max_val_loss = val_loss
                        self.best_model_loss_ = save_file_path
                        count_drop_loss = 0
                    else:
                        count_drop_loss += 1


                    if val_acc > max_val_acc:
                        max_val_acc = val_acc
                        self.best_model_acc_ = save_file_path
                        count_drop_acc = 0
                    else:
                        count_drop_acc += 1

                    print("epoch: {:<3}Training accuracy: {:<6.4f}, \n\
              Training loss: {:<6.6f}\n\
                                        Validation accuracy: {:<6.4f}\n\
                                            Validation loss: {:<6.6f}"
                      .format(epoch, train_acc, train_loss, 
                              val_acc, val_loss))
                    if count_drop_acc > self.max_stop and \
                                            count_drop_loss > self.max_stop:
                        print("Early stopping!\n    Count acc = %d\n    Count loss = %d" \
                              % (count_drop_acc, count_drop_loss))
                        self._restore_params(model)
                        break
                else:
                    print("epoch: {:<3}Training accuracy: {:<6.4f}, \n\
                              Training loss: {:<6.6f}\n".
                          format(epoch, train_acc, train_loss))

        self._file_writer.close()
        
    
    def _restore_params(self, model):
        """
        Restore the parameter from save files
        
        Parameters:
        model:
            The path to the save file or string /"acc/" or /"loss/"
            
        Return: None
        """
        with self._session.as_default() as sess:
            if model == "acc":
                self._saver.restore(sess, self.best_model_acc_)
            elif model == "loss":
                self._saver.restore(sess, self.best_model_loss_)
            else:
                try:
                     self._saver.restore(sess, model)
                except:
                    raise AttributeError("Restore for '%s' is invalid" 
                                         % str(model))

    
    def predict_proba(self, X):
        """
        Return the probability of labels of Data given
        
        Parameters:
        X:
            Dataset same features as X in fit function
            
        return: 
            probability matrix
        """
        if self._session is None:
            raise NotFittedError("This %s instamance is not fitted yet" 
                                 % self.__class__.__name__)
        with self._session.as_default() as sess:
            return self._y_proba.eval(feed_dict={self._X: X})
        
    def predict(self, X, onehot=True):
        """
        Return the predicted labels of Data given
        
        Parameters:
        X:
            Dataset same features as X in fit function
        
        onehot:
            True or False, defaults to True
            Whether the returned label is in onehot encoding or not.
            
        return: 
            Onehot prediction matrix or predicted clases number 
            (range 0 to n_labels)
        """
        if self._session is None:
            raise NotFittedError("This %s instance is not fitted yet" 
                                 % self.__class__.__name__)
        with self._session.as_default() as sess:
            if onehot:
                pred_proba = self._y_proba.eval(feed_dict={self._X: X})
                return (pred_proba ==
                        pred_proba.max(axis=1)[:,None]).astype(int)
            return self._y_pred.eval(feed_dict={self._X: X})